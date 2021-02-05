#include "layer_stack.hpp"

namespace Andromeda {
    Layer_Stack::~Layer_Stack() {
        for (Layer * layer : m_Layers) {
            layer->on_detach();
            delete layer;
        }
    }

    void Layer_Stack::push_layer(Layer * layer) {
        m_Layers.emplace(m_Layers.begin() + m_Layer_Insert_Index, layer);
        m_Layer_Insert_Index++;
        layer->on_attach();
    }

    void Layer_Stack::pop_layer(Layer * layer) {
        auto result = std::find(m_Layers.begin(), m_Layers.begin() + m_Layer_Insert_Index, layer);
        if (result != m_Layers.begin() + m_Layer_Insert_Index) {
            layer->on_detach();
            m_Layers.erase(result);
            m_Layer_Insert_Index--;
        }
    }

    void Layer_Stack::push_overlay(Layer * overlay) {
        m_Layers.emplace_back(overlay);
        overlay->on_attach();
    }

    void Layer_Stack::pop_overlay(Layer * overlay) {
        auto result = std::find(m_Layers.begin() + m_Layer_Insert_Index, m_Layers.begin(), overlay);
        if (result != m_Layers.end()) {
            overlay->on_detach();
            m_Layers.erase(result);
        }
    }
} /* Andromeda */
