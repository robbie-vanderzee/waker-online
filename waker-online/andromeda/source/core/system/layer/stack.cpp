#include "stack.hpp"

namespace Andromeda {
    Layer::Stack::~Stack() {
        std::ranges::for_each(std::begin(m_Layers), std::end(m_Layers), [](const auto & layer) {
            layer->on_detach();
        });
    }

    void Layer::Stack::push_layer(std::shared_ptr<Layer> layer) {
        layer->on_attach();
        m_Layers.emplace(std::begin(m_Layers) + m_Layer_Insert_Index, std::move(layer));
        m_Layer_Insert_Index++;
    }

    void Layer::Stack::pop_layer(std::shared_ptr<Layer> layer) {
        auto result = std::ranges::find(std::begin(m_Layers), std::begin(m_Layers) + m_Layer_Insert_Index, layer);
        if (result != m_Layers.begin() + m_Layer_Insert_Index) {
            layer->on_detach();
            m_Layers.erase(result);
            m_Layer_Insert_Index--;
        }
    }

    void Layer::Stack::push_overlay(std::shared_ptr<Layer> overlay) {
        overlay->on_attach();
        m_Layers.emplace_back(std::move(overlay));
    }

    void Layer::Stack::pop_overlay(std::shared_ptr<Layer> overlay) {
        auto result = std::ranges::find(std::begin(m_Layers) + m_Layer_Insert_Index, std::end(m_Layers), overlay);
        if (result != std::end(m_Layers))  {
            overlay->on_attach();
            m_Layers.erase(result);
        }
    }

    std::vector<std::shared_ptr<Layer>>::iterator Layer::Stack::begin() noexcept {
        return m_Layers.begin();
    }
    std::vector<std::shared_ptr<Layer>>::iterator Layer::Stack::end() noexcept {
        return m_Layers.end();
    }
    std::vector<std::shared_ptr<Layer>>::reverse_iterator Layer::Stack::rbegin() noexcept {
        return m_Layers.rbegin();
    }
    std::vector<std::shared_ptr<Layer>>::reverse_iterator Layer::Stack::rend() noexcept {
        return m_Layers.rend();
    }

    constexpr std::vector<std::shared_ptr<Layer>>::const_iterator Layer::Stack::begin() const noexcept {
        return m_Layers.begin();
    }
    constexpr std::vector<std::shared_ptr<Layer>>::const_iterator Layer::Stack::end() const noexcept {
        return m_Layers.end();
    }
    constexpr std::vector<std::shared_ptr<Layer>>::const_reverse_iterator Layer::Stack::rbegin() const noexcept {
        return m_Layers.rbegin();
    }
    constexpr std::vector<std::shared_ptr<Layer>>::const_reverse_iterator Layer::Stack::rend() const noexcept {
        return m_Layers.rend();
    }


} /* Andromeda */
