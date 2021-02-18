#include "api.hpp"
#include "core/instance.hpp"

#include <fstream>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {

            API::API(Graphics::Info info) : m_Info(info) {

            }

            void API::initialize() {
                ANDROMEDA_CORE_INFO("Initializing Vulkan API.");
                generate_vulkan_instance();
            }

            void API::process() {
                select_physical_device();
                create_logical_device();
                create_swap_chain();
                create_image_views();
                create_render_pass();
                create_graphics_pipeline();
                create_frame_buffers();
                create_command_pool();
                create_command_buffers();
                create_sync_objects();
            }

            void API::shutdown() {
                ANDROMEDA_CORE_INFO("Terminating Vulkan API.");
                vkDeviceWaitIdle(m_API_Instance.logical_device);
                for (std::size_t i = 0; i < m_API_Instance.imageAvailableSemaphores.size(); i++) {
                    vkDestroySemaphore(m_API_Instance.logical_device, m_API_Instance.imageAvailableSemaphores[i], nullptr);
                    vkDestroySemaphore(m_API_Instance.logical_device, m_API_Instance.renderFinishedSemaphores[i], nullptr);
                    vkDestroyFence(m_API_Instance.logical_device, m_API_Instance.inFlightFences[i], nullptr);
                }
                vkDestroyCommandPool(m_API_Instance.logical_device, m_API_Instance.commandPool, nullptr);
                std::ranges::for_each(m_API_Instance.swapChainFrameBuffers, [this](auto framebuffer) {
                    vkDestroyFramebuffer(m_API_Instance.logical_device, framebuffer, nullptr);
                });
                vkDestroyRenderPass(m_API_Instance.logical_device, m_API_Instance.renderPass, nullptr);
                vkDestroyPipeline(m_API_Instance.logical_device, m_API_Instance.pipeline, nullptr);
                vkDestroyPipelineLayout(m_API_Instance.logical_device, m_API_Instance.pipeline_layout, nullptr);
                std::ranges::for_each(m_API_Instance.swap_chain_image_views, [this](auto view) {
                    vkDestroyImageView(m_API_Instance.logical_device, view, nullptr);
                });
                vkDestroySwapchainKHR(m_API_Instance.logical_device, m_API_Instance.swap_chain, nullptr);
                if (m_Context) m_Context->shutdown();
                vkDestroyDevice(m_API_Instance.logical_device, nullptr);
                vkDestroyInstance(m_API_Instance.instance, nullptr);
            }

            void API::set_window_context(std::shared_ptr<Window> window) {
                m_Context = Graphics::Renderer::Context::create_context(API::Type::Vulkan, window);
                m_Context->initialize(m_API_Instance.instance);
            }

            void API::generate_vulkan_instance() {
                create_application_info();
                create_instance_create_info();
                m_API_Instance.instance_status = create_instance();
                ANDROMEDA_CORE_ASSERT(m_API_Instance.instance_status == VK_SUCCESS, "Failed to create Vulkan API instance.");
#ifdef DEBUG
                std::vector<VkExtensionProperties> instance_extension_properties;
                VkResult enumerate_instance_extension_properties_status = enumerate_instance_extension_properties(instance_extension_properties);
                ANDROMEDA_CORE_ASSERT(enumerate_instance_extension_properties_status == VK_SUCCESS, "Failed to enumerate Vulkan extension layer properties.");
                ANDROMEDA_CORE_INFO("Vulkan Extensions: ");
                for (auto & instance_extension_property : instance_extension_properties) ANDROMEDA_CORE_TRACE("{0}", instance_extension_property.extensionName);
#endif
            }

            void API::select_physical_device() {
                m_API_Instance.physical_device = VK_NULL_HANDLE;
                std::vector<VkPhysicalDevice> physical_devices;
                VkResult enumerate_physical_devices_status = enumerate_physical_devices(physical_devices);
                ANDROMEDA_CORE_ASSERT(enumerate_physical_devices_status == VK_SUCCESS, "Failed to enumerate Vulkan physical devices.");
#ifdef DEBUG
                ANDROMEDA_CORE_INFO("Physical Devices: ");
                VkPhysicalDeviceProperties physical_device_properties;
                for (auto & physical_device : physical_devices) {
                    vkGetPhysicalDeviceProperties(physical_device, & physical_device_properties);
                    ANDROMEDA_CORE_TRACE("{0}", physical_device_properties.deviceName);
                }
#endif
                std::multimap<unsigned int, VkPhysicalDevice> sorted_physical_devices;
                for (auto physical_device : physical_devices) {
                    sorted_physical_devices.emplace(evaluate_physical_device(physical_device), physical_device);
                }
                auto best_physical_device = sorted_physical_devices.begin();
                m_API_Instance.physical_device = best_physical_device->second;
                ANDROMEDA_CORE_ASSERT(best_physical_device->first > 0, "No physical device sufficiently supports required Vulkan features.");
                vkGetPhysicalDeviceProperties(m_API_Instance.physical_device, & m_API_Instance.physical_device_properties);
                vkGetPhysicalDeviceFeatures(m_API_Instance.physical_device, & m_API_Instance.physical_device_features);
                get_queue_family_properties(m_API_Instance.queue_family_properties);
                verify_queue_family_properties();
                if (m_Context) {
                    VkSurfaceKHR surface = std::any_cast<VkSurfaceKHR> (m_Context->get_native_context());
                    auto result = std::find_if(m_API_Instance.queue_family_properties.begin(), m_API_Instance.queue_family_properties.end(), [this, surface, index = 0](const auto &) mutable {
                        VkBool32 supported = VK_FALSE;
                        auto get_physical_device_surface_support_KHR_status = get_physical_device_surface_support_KHR(m_API_Instance.physical_device, index++, surface, & supported);
                        ANDROMEDA_CORE_ASSERT(get_physical_device_surface_support_KHR_status == VK_SUCCESS, "Failed to get physical device surface support KHR");
                        return supported == VK_TRUE;
                    });
                    ANDROMEDA_CORE_ASSERT(result != m_API_Instance.queue_family_properties.end(), "Failed to find a present capable queue!");
                    m_API_Instance.present_queue_index = std::distance(result, m_API_Instance.queue_family_properties.begin());
                } else {
                    ANDROMEDA_CORE_INFO("No present queue selected.");
                }
                bool suitable = verify_device_is_suitable();
                ANDROMEDA_CORE_ASSERT(suitable, "Device is not suitable for use; lacking surface requirements.");
                ANDROMEDA_CORE_INFO("Selected Device: {0}", m_API_Instance.physical_device_properties.deviceName);
            }

            void API::create_logical_device() {
                float priority = 1.0f;
                create_device_queue_create_info(priority);
                create_device_create_info();
                m_API_Instance.logical_device_status = create_device();
                ANDROMEDA_CORE_ASSERT(m_API_Instance.logical_device_status == VK_SUCCESS, "Failed to create Vulkan logical device.");
                vkGetDeviceQueue(m_API_Instance.logical_device, m_API_Instance.graphics_queue_index.value(), 0, & m_API_Instance.graphics_queue);
                vkGetDeviceQueue(m_API_Instance.logical_device, m_API_Instance.present_queue_index.value(), 0, & m_API_Instance.present_queue);
            }

            void API::create_swap_chain() {
                Surface_Capabilities swap_chain_support = get_physical_device_capabilities(m_API_Instance.physical_device);
                VkSurfaceFormatKHR surface_format = select_swap_surface_format(swap_chain_support.formats);
                VkPresentModeKHR present_mode = select_swap_present_mode(swap_chain_support.present_modes);
                VkExtent2D extent = select_swap_extent(swap_chain_support.capabilities);
                unsigned int image_count = swap_chain_support.capabilities.minImageCount + 1;
                image_count = std::min(image_count, swap_chain_support.capabilities.maxImageCount);

                VkSwapchainCreateInfoKHR create_swap_chain_create_info{};
                create_swap_chain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
                create_swap_chain_create_info.surface = std::any_cast<VkSurfaceKHR>(m_Context->get_native_context());
                create_swap_chain_create_info.minImageCount = image_count;
                create_swap_chain_create_info.imageFormat = surface_format.format;
                create_swap_chain_create_info.imageColorSpace = surface_format.colorSpace;
                create_swap_chain_create_info.imageExtent = extent;
                create_swap_chain_create_info.imageArrayLayers = 1;
                create_swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

                create_swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                create_swap_chain_create_info.queueFamilyIndexCount = 1;
                create_swap_chain_create_info.pQueueFamilyIndices = nullptr;
                unsigned int queue_family_indices[] = {m_API_Instance.graphics_queue_index.value(), m_API_Instance.present_queue_index.value()};
                if (m_API_Instance.graphics_queue_index.value() != m_API_Instance.present_queue_index.value()) {
                    create_swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                    create_swap_chain_create_info.queueFamilyIndexCount = 2;
                    create_swap_chain_create_info.pQueueFamilyIndices = queue_family_indices;
                }
                create_swap_chain_create_info.preTransform = swap_chain_support.capabilities.currentTransform;
                create_swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
                create_swap_chain_create_info.presentMode = present_mode;
                create_swap_chain_create_info.clipped = VK_TRUE;
                create_swap_chain_create_info.oldSwapchain = VK_NULL_HANDLE;

                m_API_Instance.create_swap_chain_create_info = create_swap_chain_create_info;
                VkResult create_swap_chain_status = create_vk_swap_chain();
                ANDROMEDA_CORE_ASSERT(create_swap_chain_status == VK_SUCCESS, "Failed to create swap chain.");
                VkResult get_swap_chain_images_status = get_swap_chain_images_KHR(m_API_Instance.swap_chain_images);
                ANDROMEDA_CORE_ASSERT(get_swap_chain_images_status == VK_SUCCESS, "Failed to get swap chain images.");
                m_API_Instance.swap_chain_extent = extent;
                m_API_Instance.swap_chain_image_format = surface_format.format;
            }

            void API::create_image_views() {
                m_API_Instance.swap_chain_image_views.resize(m_API_Instance.swap_chain_images.size());
                std::ranges::for_each(m_API_Instance.swap_chain_images, [this, index = 0](const auto & swap_chain_image) mutable {
                    VkImageViewCreateInfo create_image_view_info{};
                    create_image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                    create_image_view_info.pNext = nullptr;
                    create_image_view_info.flags = 0;
                    create_image_view_info.image = swap_chain_image;
                    create_image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
                    create_image_view_info.format = m_API_Instance.swap_chain_image_format;

                    create_image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                    create_image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                    create_image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                    create_image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

                    create_image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    create_image_view_info.subresourceRange.baseMipLevel = 0;
                    create_image_view_info.subresourceRange.levelCount = 1;
                    create_image_view_info.subresourceRange.baseArrayLayer = 0;
                    create_image_view_info.subresourceRange.layerCount = 1;
                    auto create_image_view_status = create_image_view(&m_API_Instance.swap_chain_image_views[index++], &create_image_view_info);
                    ANDROMEDA_CORE_INFO("View {0}", index);
                    ANDROMEDA_CORE_ASSERT(create_image_view_status == VK_SUCCESS, "Failed to create image view.");
                });

            }

            void API::create_graphics_pipeline() {
                ANDROMEDA_CORE_INFO("Loading shaders");
                auto vertshaderbin = load_shader("./waker-online/waker-online/assets/shaders/compiled/test/test.vert.spv");
                auto fragshaderbin = load_shader("./waker-online/waker-online/assets/shaders/compiled/test/test.frag.spv");
                ANDROMEDA_CORE_ASSERT(!vertshaderbin.empty() && !fragshaderbin.empty(), "Failed to loaded shaders");
                VkShaderModule vertshadermodule = create_shader_module(vertshaderbin);
                VkShaderModule fragshadermodule = create_shader_module(fragshaderbin);

                VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
                vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vert_shader_stage_info.module = vertshadermodule;
                vert_shader_stage_info.pName = "main";


                VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
                frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                frag_shader_stage_info.module = fragshadermodule;
                frag_shader_stage_info.pName = "main";

                VkPipelineShaderStageCreateInfo shaderStages[] = {vert_shader_stage_info, frag_shader_stage_info};

                VkPipelineVertexInputStateCreateInfo vertex_input_info{};
                vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertex_input_info.vertexBindingDescriptionCount = 0;
                vertex_input_info.pVertexBindingDescriptions = nullptr;
                vertex_input_info.vertexAttributeDescriptionCount = 0;
                vertex_input_info.pVertexAttributeDescriptions = nullptr;

                VkViewport viewport{};
                viewport.x = 0.0f;
                viewport.y = 0.0f;
                viewport.width = (float) m_API_Instance.swap_chain_extent.width;
                viewport.height = (float) m_API_Instance.swap_chain_extent.height;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;

                VkRect2D scissor{};
                scissor.offset = {0, 0};
                scissor.extent = m_API_Instance.swap_chain_extent;

                VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
                inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                inputAssembly.primitiveRestartEnable = VK_FALSE;

                VkPipelineViewportStateCreateInfo viewportState{};
                viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportState.viewportCount = 1;
                viewportState.pViewports = &viewport;
                viewportState.scissorCount = 1;
                viewportState.pScissors = &scissor;

                VkPipelineRasterizationStateCreateInfo rasterizer{};
                rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizer.depthClampEnable = VK_FALSE;
                rasterizer.rasterizerDiscardEnable = VK_FALSE;
                rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
                rasterizer.lineWidth = 1.0f;
                rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
                rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
                rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
                rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

                VkPipelineMultisampleStateCreateInfo multisampling{};
                multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampling.sampleShadingEnable = VK_FALSE;
                multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
                multisampling.minSampleShading = 1.0f;
                multisampling.pSampleMask = nullptr;
                multisampling.alphaToCoverageEnable = VK_FALSE;
                multisampling.alphaToOneEnable = VK_FALSE;

                VkPipelineColorBlendAttachmentState colorBlendAttachment{};
                colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
                colorBlendAttachment.blendEnable = VK_TRUE;
                colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
                colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

                VkPipelineColorBlendStateCreateInfo colorBlending{};
                colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                colorBlending.logicOpEnable = VK_FALSE;
                colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
                colorBlending.attachmentCount = 1;
                colorBlending.pAttachments = &colorBlendAttachment;
                colorBlending.blendConstants[0] = 0.0f; // Optional
                colorBlending.blendConstants[1] = 0.0f; // Optional
                colorBlending.blendConstants[2] = 0.0f; // Optional
                colorBlending.blendConstants[3] = 0.0f; // Optional

                VkDynamicState dynamicStates[] = {
                    VK_DYNAMIC_STATE_VIEWPORT,
                    VK_DYNAMIC_STATE_LINE_WIDTH
                };

                VkPipelineDynamicStateCreateInfo dynamicState{};
                dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamicState.dynamicStateCount = 2;
                dynamicState.pDynamicStates = dynamicStates;

                VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
                pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                pipelineLayoutInfo.setLayoutCount = 0; // Optional
                pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
                pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
                pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

                auto pipeline_layout_status = vkCreatePipelineLayout(m_API_Instance.logical_device, & pipelineLayoutInfo, nullptr, & m_API_Instance.pipeline_layout);
                ANDROMEDA_CORE_ASSERT(pipeline_layout_status == VK_SUCCESS, "Failed to create pipeline layout");

                VkGraphicsPipelineCreateInfo pipelineInfo{};
                pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                pipelineInfo.stageCount = 2;
                pipelineInfo.pStages = shaderStages;
                pipelineInfo.pVertexInputState = &vertex_input_info;
                pipelineInfo.pInputAssemblyState = &inputAssembly;
                pipelineInfo.pViewportState = &viewportState;
                pipelineInfo.pRasterizationState = &rasterizer;
                pipelineInfo.pMultisampleState = &multisampling;
                pipelineInfo.pDepthStencilState = nullptr; // Optional
                pipelineInfo.pColorBlendState = &colorBlending;
                pipelineInfo.pDynamicState = nullptr; // Optional
                pipelineInfo.layout = m_API_Instance.pipeline_layout;
                pipelineInfo.renderPass = m_API_Instance.renderPass;
                pipelineInfo.subpass = 0;

                auto graphics_pipeline_status = vkCreateGraphicsPipelines(m_API_Instance.logical_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_API_Instance.pipeline);
                ANDROMEDA_CORE_ASSERT(graphics_pipeline_status == VK_SUCCESS, "Failed to create graphics pipeline");
                vkDestroyShaderModule(m_API_Instance.logical_device, vertshadermodule, nullptr);
                vkDestroyShaderModule(m_API_Instance.logical_device, fragshadermodule, nullptr);
            }

            void API::create_render_pass() {
                VkAttachmentDescription colorAttachment{};
                colorAttachment.format = m_API_Instance.swap_chain_image_format;
                colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
                colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

                colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

                colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                VkAttachmentReference colorAttachmentRef{};
                colorAttachmentRef.attachment = 0;
                colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass{};
                subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass.colorAttachmentCount = 1;
                subpass.pColorAttachments = &colorAttachmentRef;

                VkRenderPassCreateInfo renderPassInfo{};
                renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
                renderPassInfo.attachmentCount = 1;
                renderPassInfo.pAttachments = &colorAttachment;
                renderPassInfo.subpassCount = 1;
                renderPassInfo.pSubpasses = &subpass;

                VkSubpassDependency dependency{};
                dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
                dependency.dstSubpass = 0;

                dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                dependency.srcAccessMask = 0;
                dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

                renderPassInfo.dependencyCount = 1;
                renderPassInfo.pDependencies = &dependency;

                auto status = vkCreateRenderPass(m_API_Instance.logical_device, &renderPassInfo, nullptr, &m_API_Instance.renderPass);
                ANDROMEDA_CORE_ASSERT(status == VK_SUCCESS, "Failed to create render pass");

            }

            void API::create_frame_buffers() {
                m_API_Instance.swapChainFrameBuffers.resize(m_API_Instance.swap_chain_image_views.size());
                std::ranges::for_each(m_API_Instance.swap_chain_image_views, [ &, index = 0](auto image) mutable {
                    VkImageView attachments[] = { image };

                    VkFramebufferCreateInfo framebufferInfo{};
                    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    framebufferInfo.renderPass = m_API_Instance.renderPass;
                    framebufferInfo.attachmentCount = 1;
                    framebufferInfo.pAttachments = attachments;
                    framebufferInfo.width = m_API_Instance.swap_chain_extent.width;
                    framebufferInfo.height = m_API_Instance.swap_chain_extent.height;
                    framebufferInfo.layers = 1;

                    auto status = vkCreateFramebuffer(m_API_Instance.logical_device, &framebufferInfo, nullptr, &m_API_Instance.swapChainFrameBuffers[index++]);
                    ANDROMEDA_CORE_INFO("Frame buffer {0}", index);
                    ANDROMEDA_CORE_ASSERT(status == VK_SUCCESS, "Failed to create frame buffer {0}", index);
                });
            }

            void API::create_command_pool() {
                auto graphics_family = std::ranges::find_if(m_API_Instance.queue_family_properties, [](const auto & property) {
                    return property.queueFlags & VK_QUEUE_GRAPHICS_BIT;
                });

                VkCommandPoolCreateInfo poolInfo{};
                poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
                poolInfo.queueFamilyIndex = std::distance(graphics_family, std::begin(m_API_Instance.queue_family_properties));
                poolInfo.flags = 0; // Optional
                ANDROMEDA_CORE_INFO("Queue familly found at index {0}", poolInfo.queueFamilyIndex);
                auto status = vkCreateCommandPool(m_API_Instance.logical_device, &poolInfo, nullptr, &m_API_Instance.commandPool);
                ANDROMEDA_CORE_ASSERT(status == VK_SUCCESS, "Failed to create command pool.");
            }

            void API::create_command_buffers() {
                m_API_Instance.commandBuffers.resize(m_API_Instance.swapChainFrameBuffers.size());
                VkCommandBufferAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                allocInfo.commandPool = m_API_Instance.commandPool;
                allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                allocInfo.commandBufferCount = m_API_Instance.commandBuffers.size();

                auto status = vkAllocateCommandBuffers(m_API_Instance.logical_device, &allocInfo, m_API_Instance.commandBuffers.data());
                ANDROMEDA_CORE_ASSERT(status == VK_SUCCESS, "Failed to create command buffers.");

                for (std::size_t index = 0; index < m_API_Instance.commandBuffers.size(); index++) {
                    VkCommandBufferBeginInfo beginInfo{};
                    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                    beginInfo.flags = 0; // Optional
                    beginInfo.pInheritanceInfo = nullptr; // Optional
                    auto status = vkBeginCommandBuffer(m_API_Instance.commandBuffers[index], &beginInfo);
                    ANDROMEDA_CORE_ASSERT(status == VK_SUCCESS, "Failed to begin recording command buffer.");

                    VkRenderPassBeginInfo renderPassInfo{};
                    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                    renderPassInfo.renderPass = m_API_Instance.renderPass;
                    renderPassInfo.framebuffer = m_API_Instance.swapChainFrameBuffers[index];
                    renderPassInfo.renderArea.offset = {0, 0};
                    renderPassInfo.renderArea.extent = m_API_Instance.swap_chain_extent;

                    VkClearValue clearColor = { .color {.float32 = {0.0f, 0.0f, 0.0f, 1.0f}}};
                    renderPassInfo.clearValueCount = 1;
                    renderPassInfo.pClearValues = &clearColor;
                    vkCmdBeginRenderPass(m_API_Instance.commandBuffers[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
                    vkCmdBindPipeline(m_API_Instance.commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_API_Instance.pipeline);
                    vkCmdDraw(m_API_Instance.commandBuffers[index], 3, 1, 0, 0);
                    vkCmdEndRenderPass(m_API_Instance.commandBuffers[index]);
                    auto end = vkEndCommandBuffer(m_API_Instance.commandBuffers[index]);
                    ANDROMEDA_CORE_ASSERT(end == VK_SUCCESS, "Failed to bind cmd to pipeline {0}", index);
                };
            }

            void API::create_sync_objects() {

                m_API_Instance.renderFinishedSemaphores.resize(m_API_Instance.max_frames_in_flight);
                m_API_Instance.imageAvailableSemaphores.resize(m_API_Instance.max_frames_in_flight);
                m_API_Instance.inFlightFences.resize(m_API_Instance.max_frames_in_flight);
                m_API_Instance.imagesInFlight.resize(m_API_Instance.swap_chain_images.size(), VK_NULL_HANDLE);

                VkSemaphoreCreateInfo semaphoreInfo{};
                semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

                VkFenceCreateInfo fenceInfo{};
                fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
                fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

                for (std::size_t i = 0; i < m_API_Instance.imageAvailableSemaphores.size(); i++) {
                    auto renderstatus = vkCreateSemaphore(m_API_Instance.logical_device, &semaphoreInfo, nullptr, &m_API_Instance.renderFinishedSemaphores[i]);
                    auto imagestatus = vkCreateSemaphore(m_API_Instance.logical_device, &semaphoreInfo, nullptr, &m_API_Instance.imageAvailableSemaphores[i]);
                    ANDROMEDA_CORE_ASSERT(renderstatus == VK_SUCCESS && imagestatus == VK_SUCCESS, "Failed semaphore creation.");
                    auto fencestatus = vkCreateFence(m_API_Instance.logical_device, &fenceInfo, nullptr, &m_API_Instance.inFlightFences[i]);
                    ANDROMEDA_CORE_ASSERT(fencestatus == VK_SUCCESS && imagestatus == VK_SUCCESS, "Failed fence creation.");
                }
            }

            void API::present() {
                vkWaitForFences(m_API_Instance.logical_device, 1, &m_API_Instance.inFlightFences[m_API_Instance.current_frame], VK_TRUE, UINT64_MAX);

                unsigned int index;
                vkAcquireNextImageKHR(m_API_Instance.logical_device, m_API_Instance.swap_chain, UINT64_MAX, m_API_Instance.imageAvailableSemaphores[m_API_Instance.current_frame], VK_NULL_HANDLE, &index);

                if (m_API_Instance.imagesInFlight[m_API_Instance.current_frame] != VK_NULL_HANDLE) {
                    vkWaitForFences(m_API_Instance.logical_device, 1, &m_API_Instance.imagesInFlight[m_API_Instance.current_frame], VK_TRUE, UINT64_MAX);
                }

                m_API_Instance.imagesInFlight[m_API_Instance.current_frame] = m_API_Instance.inFlightFences[m_API_Instance.current_frame];

                VkSubmitInfo submitInfo{};
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

                VkSemaphore waitSemaphores[] = {m_API_Instance.imageAvailableSemaphores[m_API_Instance.current_frame]};
                VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
                submitInfo.waitSemaphoreCount = 1;
                submitInfo.pWaitSemaphores = waitSemaphores;
                submitInfo.pWaitDstStageMask = waitStages;
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers = &m_API_Instance.commandBuffers[index];

                VkSemaphore signalSemaphores[] = {m_API_Instance.renderFinishedSemaphores[m_API_Instance.current_frame]};
                submitInfo.signalSemaphoreCount = 1;
                submitInfo.pSignalSemaphores = signalSemaphores;

                vkResetFences(m_API_Instance.logical_device, 1, &m_API_Instance.imagesInFlight[m_API_Instance.current_frame]);

                auto submit = vkQueueSubmit(m_API_Instance.graphics_queue, 1, &submitInfo, m_API_Instance.inFlightFences[m_API_Instance.current_frame]);
                ANDROMEDA_CORE_ASSERT(submit == VK_SUCCESS, "Failed to submit to graphics queue.");

                VkPresentInfoKHR presentInfo{};
                presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

                presentInfo.waitSemaphoreCount = 1;
                presentInfo.pWaitSemaphores = signalSemaphores;

                VkSwapchainKHR swapChains[] = {m_API_Instance.swap_chain};
                presentInfo.swapchainCount = 1;
                presentInfo.pSwapchains = swapChains;
                presentInfo.pImageIndices = &index;
                presentInfo.pResults = nullptr; // Optional
                vkQueuePresentKHR(m_API_Instance.present_queue, &presentInfo);
                m_API_Instance.current_frame = (m_API_Instance.current_frame + 1) % m_API_Instance.max_frames_in_flight;
            }

            void API::create_application_info() {
                m_API_Instance.application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                m_API_Instance.application_info.pNext = nullptr;
                m_API_Instance.application_info.pApplicationName = m_Info.application.c_str();
                m_API_Instance.application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                m_API_Instance.application_info.pEngineName = m_Info.engine.c_str();
                m_API_Instance.application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                m_API_Instance.application_info.apiVersion = VK_MAKE_VERSION(1, 2, 0);
            }


            void API::create_instance_create_info() {
                unsigned int glfw_instance_extension_count = 0;
                const char ** glfw_instance_extensions = glfwGetRequiredInstanceExtensions(& glfw_instance_extension_count);
                m_API_Instance.instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                m_API_Instance.instance_create_info.pNext = nullptr;
                m_API_Instance.instance_create_info.flags = 0;
                m_API_Instance.instance_create_info.pApplicationInfo = & m_API_Instance.application_info;
                m_API_Instance.instance_create_info.enabledLayerCount = 0;
                m_API_Instance.instance_create_info.ppEnabledLayerNames = nullptr;
                m_API_Instance.instance_create_info.enabledExtensionCount  = glfw_instance_extension_count;
                m_API_Instance.instance_create_info.ppEnabledExtensionNames = glfw_instance_extensions;
#ifdef DEBUG
                bool verify_desired_validation_layer_support_status = verify_desired_validation_layer_support(desired_validation_layers);
                ANDROMEDA_CORE_ASSERT(verify_desired_validation_layer_support_status, "Failed to apply all Vulkan validation layers.");
                m_API_Instance.instance_create_info.enabledLayerCount = desired_validation_layers.size();
                m_API_Instance.instance_create_info.ppEnabledLayerNames = desired_validation_layers.data();
                ANDROMEDA_CORE_INFO("Enabled Vulkan Validation Layers: ");
                for (auto & enabled_validation_layer : desired_validation_layers) ANDROMEDA_CORE_TRACE("{0}", enabled_validation_layer);
#endif
            }

            VkResult API::create_instance() {
                VkResult create_instance_status = vkCreateInstance(& m_API_Instance.instance_create_info, nullptr, & m_API_Instance.instance);
                switch (create_instance_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully initialized Vulkan API Instance.");
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Device out of memory.");
                        break;
                    case VK_ERROR_INITIALIZATION_FAILED:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Initialization failed.");
                        break;
                    case VK_ERROR_LAYER_NOT_PRESENT:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Validation Layers not present.");
                        break;
                    case VK_ERROR_EXTENSION_NOT_PRESENT:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Extensions not present.");
                        break;
                    case VK_ERROR_INCOMPATIBLE_DRIVER:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Incompatible Driver.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled Vulkan create instance result: {0}.", create_instance_status);
                        break;
                }
                return create_instance_status;
            }

            VkResult API::create_device() {
                VkResult create_device_status = vkCreateDevice(m_API_Instance.physical_device, &m_API_Instance.device_create_info, nullptr, &m_API_Instance.logical_device);
                switch (create_device_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully initialized Vulkan API Device.");
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Device out of memory.");
                        break;
                    case VK_ERROR_INITIALIZATION_FAILED:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Initialization failed.");
                        break;
                    case VK_ERROR_EXTENSION_NOT_PRESENT:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Extensions not present.");
                        break;
                    case VK_ERROR_FEATURE_NOT_PRESENT:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Features not present.");
                        break;
                    case VK_ERROR_TOO_MANY_OBJECTS:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Too many objects.");
                        break;
                    case VK_ERROR_DEVICE_LOST:
                        ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. The device was lost.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled Vulkan create device result: {0}.", create_device_status);
                        break;
                }
                return create_device_status;
            }

            VkResult API::create_vk_swap_chain() {
                VkResult create_swap_chain_status = vkCreateSwapchainKHR(m_API_Instance.logical_device, &m_API_Instance.create_swap_chain_create_info, nullptr, &m_API_Instance.swap_chain);
                switch (create_swap_chain_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully created Vulkan swap chain.");
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Device out of memory.");
                        break;
                    case VK_ERROR_DEVICE_LOST:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Device lost.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Surface lost.");
                        break;
                    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Native window in use.");
                        break;
                    case VK_ERROR_INITIALIZATION_FAILED:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Initialization failed.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled Vulkan create device result: {0}.", create_swap_chain_status);
                        break;
                }
                return create_swap_chain_status;
            }


            VkResult API::create_image_view(VkImageView * view, VkImageViewCreateInfo * info) {
                VkResult create_image_view_status = vkCreateImageView(m_API_Instance.logical_device, info, nullptr, view);
                switch (create_image_view_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully created image view.");
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to create image view. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to create image view. Device out of memory.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled Vulkan create device result: {0}.", create_image_view_status);
                        break;
                }
                return create_image_view_status;
            }

            VkResult API::enumerate_instance_extension_properties(std::vector<VkExtensionProperties> & instance_extension_properties) {
                unsigned int instance_extension_properties_count;
                VkResult count_instance_extension_properties_status = vkEnumerateInstanceExtensionProperties(nullptr, & instance_extension_properties_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_instance_extension_properties_status == VK_SUCCESS, "Failed to count Vulkan extensions properties.");
                instance_extension_properties.resize(instance_extension_properties_count);
                VkResult enumerate_instance_extensions_properties_status = vkEnumerateInstanceExtensionProperties(nullptr, & instance_extension_properties_count, instance_extension_properties.data());
                ANDROMEDA_CORE_ASSERT(enumerate_instance_extensions_properties_status == VK_SUCCESS, "Failed to enumerate Vulkan extension properties.");
                switch (enumerate_instance_extensions_properties_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully enumerated extension properties.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_WARN("Failed to enumerate extension properties. Provided instance_extension_properties_count {0} was invalid.", instance_extension_properties_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate extension properties. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate extension properties. Device out of memory.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled enumerate extension result: {0}.", enumerate_instance_extensions_properties_status);
                        break;
                }
                return enumerate_instance_extensions_properties_status;
            }

            VkResult API::enumerate_instance_layer_properties(std::vector<VkLayerProperties> & available_validation_layers) {
                unsigned int layer_count;
                VkResult count_instance_layer_properties_status = vkEnumerateInstanceLayerProperties(& layer_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_instance_layer_properties_status == VK_SUCCESS, "Failed to count Vulkan validation layers.");
                available_validation_layers.resize(layer_count);
                VkResult enumerate_instance_layer_properties_status = vkEnumerateInstanceLayerProperties(& layer_count, available_validation_layers.data());
                switch (enumerate_instance_layer_properties_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully enumerated validation layer properties.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_WARN("Failed to enumerate validation layer properties. Provided layer_count {0} was invalid.", layer_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate validation layer properties. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate validation layer properties. Device out of memory.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled enumerate validation layer result: {0}.", enumerate_instance_layer_properties_status);
                        break;
                }
                return enumerate_instance_layer_properties_status;
            }

            VkResult API::enumerate_physical_device_extension_properties(std::vector<VkExtensionProperties> & device_extension_properties) {
                unsigned int device_extension_properties_count;
                VkResult count_device_extension_properties_status = vkEnumerateDeviceExtensionProperties(m_API_Instance.physical_device, nullptr, & device_extension_properties_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_device_extension_properties_status == VK_SUCCESS, "Failed to count Vulkan device extensions properties.");
                device_extension_properties.resize(device_extension_properties_count);
                VkResult enumerate_device_extensions_properties_status = vkEnumerateDeviceExtensionProperties(m_API_Instance.physical_device, nullptr, & device_extension_properties_count, device_extension_properties.data());
                ANDROMEDA_CORE_ASSERT(enumerate_device_extensions_properties_status == VK_SUCCESS, "Failed to enumerate Vulkan extension properties.");
                switch (enumerate_device_extensions_properties_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully enumerated extension properties.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_WARN("Failed to enumerate extension properties. Provided device_extension_properties_count {0} was invalid.", device_extension_properties_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate extension properties. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate extension properties. Device out of memory.");
                        break;
                    case VK_ERROR_LAYER_NOT_PRESENT:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate exxtension properties. Layer not present.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled enumerate extension result: {0}.", enumerate_device_extensions_properties_status);
                        break;
                }
                return enumerate_device_extensions_properties_status;
            }

            VkResult API::enumerate_physical_devices(std::vector<VkPhysicalDevice> & physical_devices) {
                unsigned int physical_device_count = 0;
                VkResult count_physical_devices_status = vkEnumeratePhysicalDevices(m_API_Instance.instance, & physical_device_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_physical_devices_status == VK_SUCCESS, "Failed to count Vulkan physical devices.");
                ANDROMEDA_CORE_ASSERT(physical_device_count > 0, "No physical devices found.");
                physical_devices.resize(physical_device_count);
                VkResult enumerate_physical_devices_status = vkEnumeratePhysicalDevices(m_API_Instance.instance, & physical_device_count, physical_devices.data());
                switch (enumerate_physical_devices_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully enumerated physical devices.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_WARN("Failed to enumerate physical devices. Provided physical_device_count {0} was invalid.", physical_device_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate physical devices. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate physical devices. Device out of memory.");
                        break;
                    case VK_ERROR_INITIALIZATION_FAILED:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate physical devices. Initialization failed.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled enumerate physical device result: {0}.", enumerate_physical_devices_status);
                        break;
                }
                return enumerate_physical_devices_status;
            }

            VkResult API::get_physical_device_surface_support_KHR(VkPhysicalDevice device, unsigned int queue_family_index, VkSurfaceKHR surface, VkBool32 * supported) {
                VkResult get_physical_device_surface_support_KHR_status = vkGetPhysicalDeviceSurfaceSupportKHR(device, queue_family_index, surface, supported);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_support_KHR_status == VK_SUCCESS, "Failed to determine device surface support.");
                switch (get_physical_device_surface_support_KHR_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully determined physical device surface support.");
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface support. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface support. Device out of memory.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface support. Surface lost.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled determine physical device surface support result: {0}.", get_physical_device_surface_support_KHR_status);
                        break;
                }
                return get_physical_device_surface_support_KHR_status;
            }

            VkResult API::get_physical_device_surface_capabilities_KHR(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR & capabilities) {
                VkResult get_physical_device_surface_capabilities_KHR_status = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, & capabilities);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_capabilities_KHR_status == VK_SUCCESS, "Failed to determine device surface capabilities.");
                switch (get_physical_device_surface_capabilities_KHR_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully determined physical device surface capabilities.");
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface capabilities. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface capabilities. Device out of memory.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface capabilities. Surface lost.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled determine physical device surface support result: {0}.", get_physical_device_surface_capabilities_KHR_status);
                        break;
                }
                return get_physical_device_surface_capabilities_KHR_status;
            }

            VkResult API::get_physical_device_surface_formats_KHR(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR> & formats) {
                unsigned int format_count = 0;
                VkResult count_physical_device_surface_formats_KHR_status = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, & format_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_physical_device_surface_formats_KHR_status == VK_SUCCESS, "Failed to count physical device surface formats.");
                formats.resize(format_count);
                VkResult get_physical_device_surface_formats_KHR_status = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, & format_count, formats.data());
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_formats_KHR_status == VK_SUCCESS, "Failed to get physical device surface formats.");
                switch (get_physical_device_surface_formats_KHR_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully determined physical device surface formats.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface formats. Invalid format_count {0}.", format_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface formats. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface formats. Device out of memory.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface formats. Surface lost.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled get physical device surface format result: {0}.", get_physical_device_surface_formats_KHR_status);
                        break;
                }
                return get_physical_device_surface_formats_KHR_status;
            }

            VkResult API::get_physical_device_surface_present_modes_KHR(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR> & present_modes) {
                unsigned int present_mode_count = 0;
                VkResult count_physical_device_surface_present_modes_KHR_status = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, & present_mode_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_physical_device_surface_present_modes_KHR_status == VK_SUCCESS, "Failed to count physical device surface present modes.");
                present_modes.resize(present_mode_count);
                VkResult get_physical_device_surface_present_modes_KHR_status = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, & present_mode_count, present_modes.data());
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_present_modes_KHR_status == VK_SUCCESS, "Failed to get physical device surface present modes.");
                switch (get_physical_device_surface_present_modes_KHR_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully determined physical device surface present modes.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface present modes. Invalid format_count {0}.", present_mode_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface present modes. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface present modes. Device out of memory.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface present modes. Surface lost.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled get physical device surface present modes result: {0}.", get_physical_device_surface_present_modes_KHR_status);
                        break;
                }
                return get_physical_device_surface_present_modes_KHR_status;
            }

            VkResult API::get_swap_chain_images_KHR(std::vector<VkImage> & images) {
                unsigned int image_count = 0;
                VkResult count_swap_chain_images_status = vkGetSwapchainImagesKHR(m_API_Instance.logical_device, m_API_Instance.swap_chain, &image_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_swap_chain_images_status == VK_SUCCESS, "Failed to count swap chain images.");
                images.resize(image_count);
                VkResult get_swap_chain_images_status = vkGetSwapchainImagesKHR(m_API_Instance.logical_device, m_API_Instance.swap_chain, &image_count, images.data());
                switch (get_swap_chain_images_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully obtained swap chain images.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_ERROR("Failed to get swap chain images. Invalid image_count {0}.", image_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get swap chain images. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get swap chain images. Device out of memory.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled get swap chain images result: {0}.", get_swap_chain_images_status);
                        break;
                }
                return get_swap_chain_images_status;
            }

            void API::get_queue_family_properties(std::vector<VkQueueFamilyProperties> & queue_family_properties) {
                unsigned int queue_family_properties_count = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(m_API_Instance.physical_device, & queue_family_properties_count, nullptr);
                queue_family_properties.resize(queue_family_properties_count);
                vkGetPhysicalDeviceQueueFamilyProperties(m_API_Instance.physical_device, & queue_family_properties_count, queue_family_properties.data());
            }

            unsigned int API::evaluate_physical_device(const VkPhysicalDevice & physical_device) {
                int score = 0;
                VkPhysicalDeviceProperties physical_device_properties;
                VkPhysicalDeviceFeatures physical_device_features;
                vkGetPhysicalDeviceProperties(physical_device, & physical_device_properties);
                vkGetPhysicalDeviceFeatures(physical_device, & physical_device_features);
                if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score++;
                if (!physical_device_features.geometryShader) return 0;
                score += physical_device_properties.limits.maxImageDimension3D;
                return score;
            }

            void API::verify_queue_family_properties() {
                auto result = std::find_if(m_API_Instance.queue_family_properties.begin(), m_API_Instance.queue_family_properties.end(), [](const auto & queue_family_property) {
                    return queue_family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT;
                });
                ANDROMEDA_CORE_ASSERT(result != m_API_Instance.queue_family_properties.end(), "Failed to find necessary queue families in physical device");
                m_API_Instance.graphics_queue_index = std::distance(result, m_API_Instance.queue_family_properties.begin());
            }

            Surface_Capabilities API::get_physical_device_capabilities(VkPhysicalDevice & device) {
                Surface_Capabilities capabilities;
                VkSurfaceKHR surface = std::any_cast<VkSurfaceKHR> (m_Context->get_native_context());
                auto get_physical_device_surface_capabilities_KHR_status = get_physical_device_surface_capabilities_KHR(device, surface, capabilities.capabilities);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_capabilities_KHR_status == VK_SUCCESS, "Failed to find device surface capabilities.");
                auto get_physical_device_surface_formats_KHR_status = get_physical_device_surface_formats_KHR(device, surface, capabilities.formats);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_formats_KHR_status == VK_SUCCESS, "Failed to get device surface formats.");
                auto get_physical_device_surface_present_modes_KHR_status = get_physical_device_surface_present_modes_KHR(device, surface, capabilities.present_modes);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_present_modes_KHR_status == VK_SUCCESS, "Failed to get device surface present modes.");
                return capabilities;
            }

            VkSurfaceFormatKHR API::select_swap_surface_format(const std::vector<VkSurfaceFormatKHR> & available_formats) {
                auto result = std::find_if(available_formats.begin(), available_formats.end(), [](const auto & available_format) {
                    return available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
                });
                return *result;
            }

            VkPresentModeKHR API::select_swap_present_mode(const std::vector<VkPresentModeKHR> & available_present_modes) {
                auto result = std::find_if(available_present_modes.begin(), available_present_modes.end(), [](const auto & available_present_mode) {
                    return available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR;
                });
                return result != available_present_modes.end() ? *result : VK_PRESENT_MODE_FIFO_KHR;
            }

            VkExtent2D API::select_swap_extent(const VkSurfaceCapabilitiesKHR & capabilities) {
                auto [width, height] = m_Context->get_context_extent();
                VkExtent2D extent = { width, height };
                ANDROMEDA_CORE_INFO("Created VkExtend2D [{0} x {1}]", width, height);
                extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, extent.width));
                extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, extent.height));
                return extent;
            }

            void API::create_device_queue_create_info(float & queue_priorities) {
                std::set<unsigned int> queues = {m_API_Instance.graphics_queue_index.value(), m_API_Instance.present_queue_index.value()};
                for (auto index : queues) {
                    VkDeviceQueueCreateInfo device_queue_create_info;
                    device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    device_queue_create_info.pNext = nullptr;
                    device_queue_create_info.flags = 0;
                    device_queue_create_info.queueFamilyIndex = index;
                    device_queue_create_info.queueCount = 1;
                    device_queue_create_info.pQueuePriorities = & queue_priorities;
                    m_API_Instance.device_queue_create_infos.push_back(device_queue_create_info);
                }
            }

            void API::create_device_create_info() {
                m_API_Instance.device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                m_API_Instance.device_create_info.pNext = nullptr;
                m_API_Instance.device_create_info.flags = 0;
                m_API_Instance.device_create_info.queueCreateInfoCount = m_API_Instance.device_queue_create_infos.size();
                m_API_Instance.device_create_info.pQueueCreateInfos = m_API_Instance.device_queue_create_infos.data();
                m_API_Instance.device_create_info.enabledLayerCount = 0;
                m_API_Instance.device_create_info.ppEnabledLayerNames = nullptr;
                m_API_Instance.device_create_info.enabledExtensionCount = desired_device_extensions.size();
                m_API_Instance.device_create_info.ppEnabledExtensionNames = desired_device_extensions.data();
                m_API_Instance.device_create_info.pEnabledFeatures = nullptr;
#ifdef DEBUG
                m_API_Instance.device_create_info.enabledLayerCount = m_API_Instance.instance_create_info.enabledLayerCount;
                m_API_Instance.device_create_info.ppEnabledLayerNames = m_API_Instance.instance_create_info.ppEnabledLayerNames;
#endif
            }
            bool API::verify_device_is_suitable() {
                bool extensions_supported = verify_desired_extension_support(desired_device_extensions);
                auto capabilities = get_physical_device_capabilities(m_API_Instance.physical_device);
                return m_API_Instance.present_queue_index.has_value() && m_API_Instance.graphics_queue_index.has_value() && extensions_supported && !capabilities.formats.empty() && !capabilities.present_modes.empty();
            }

            bool API::verify_desired_extension_support(const std::vector<const char *> & desired_device_extensions) {
                std::vector<VkExtensionProperties> available_extensions;
                auto enumerate_instance_extension_properties_status = enumerate_physical_device_extension_properties(available_extensions);
                ANDROMEDA_CORE_ASSERT(enumerate_instance_extension_properties_status == VK_SUCCESS, "Failed to enumerate instance extension properties.");
                for (auto & available_extension : available_extensions) ANDROMEDA_CORE_TRACE("{0}", available_extension.extensionName);
                auto result = std::all_of(desired_device_extensions.begin(), desired_device_extensions.end(), [available_extensions](const char * desired_extension) {
                    auto result = std::find_if(available_extensions.begin(), available_extensions.end(), [desired_extension](const VkExtensionProperties & vkExtension) {
                        return strcmp(desired_extension, vkExtension.extensionName) == 0;
                    });
                    if (result == available_extensions.end()) {
                        ANDROMEDA_CORE_ERROR("The {0} extension is not available.", desired_extension);
                        return false;
                    }
                    return true;
                });
                return result;
            }

            bool API::verify_desired_validation_layer_support(const std::vector<const char *> & desired_validation_layers) {
                std::vector<VkLayerProperties> available_validation_layers;
                VkResult enumerate_instance_layer_properties_status = enumerate_instance_layer_properties(available_validation_layers);
                ANDROMEDA_CORE_ASSERT(enumerate_instance_layer_properties_status == VK_SUCCESS, "Failed to enumate Vulkan validation layer properties.");
                ANDROMEDA_CORE_INFO("Available Vulkan Validation Layers: ");
                for (auto & available_validation_layer : available_validation_layers) ANDROMEDA_CORE_TRACE("{0}", available_validation_layer.layerName);
                auto result = std::all_of(desired_validation_layers.begin(), desired_validation_layers.end(), [available_validation_layers](const char * desired_validation_layer) {
                    auto result = std::find_if(available_validation_layers.begin(), available_validation_layers.end(), [desired_validation_layer](const VkLayerProperties & vkLayer) {
                        return strcmp(desired_validation_layer, vkLayer.layerName) == 0;
                    });
                    if (result == available_validation_layers.end()) {
                        ANDROMEDA_CORE_ERROR("The {0} validation layer is not available.", desired_validation_layer);
                        return false;
                    }
                    return true;
                });
                return result;
            }


            std::vector<std::byte> API::load_shader(std::filesystem::path filepath) {
                ANDROMEDA_CORE_ASSERT(!filepath.empty(), "Invalid filepath for shader.");
                std::ifstream shader_file(filepath, std::ios::binary);
                std::vector<std::byte> binary(std::filesystem::file_size(filepath));
                shader_file.read(reinterpret_cast<char *>(binary.data()), binary.size());
                return binary;
            }


            VkShaderModule API::create_shader_module(const std::vector<std::byte> & binary) {
                VkShaderModuleCreateInfo create_shader_module_info{};
                create_shader_module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                create_shader_module_info.codeSize = binary.size();
                create_shader_module_info.pCode = reinterpret_cast<const uint32_t*>(binary.data());
                VkShaderModule module;
                VkResult status = vkCreateShaderModule(m_API_Instance.logical_device, & create_shader_module_info, nullptr, & module);
                ANDROMEDA_CORE_ASSERT(status == VK_SUCCESS, "Failed to create shader module");
                return module;
            }
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
