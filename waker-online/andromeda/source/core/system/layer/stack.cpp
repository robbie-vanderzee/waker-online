#include "stack.hpp"

namespace Andromeda {
    Layer::Stack::~Stack() {
        std::ranges::for_each(std::begin(m_Layers), std::end(m_Layers), [](const auto layer) {
            layer->on_detach();
            delete layer;
        });
    }

    void Layer::Stack::push_layer(Layer * layer) {
        m_Layers.emplace(std::begin(m_Layers) + m_Layer_Insert_Index, layer);
        m_Layer_Insert_Index++;
        layer->on_attach();
    }

    void Layer::Stack::pop_layer(Layer * layer) {
        auto result = std::ranges::find(std::begin(m_Layers), std::begin(m_Layers) + m_Layer_Insert_Index, layer);
        if (result != m_Layers.begin() + m_Layer_Insert_Index) {
            layer->on_detach();
            m_Layers.erase(result);
            m_Layer_Insert_Index--;
        }
    }

    void Layer::Stack::push_overlay(Layer * overlay) {
        m_Layers.emplace_back(overlay);
        overlay->on_attach();
    }

    void Layer::Stack::pop_overlay(Layer * overlay) {
        auto result = std::ranges::find(std::begin(m_Layers) + m_Layer_Insert_Index, std::end(m_Layers), overlay);
        if (result != std::end(m_Layers))  {
            overlay->on_attach();
            m_Layers.erase(result);
        }
    }
} /* Andromeda */
