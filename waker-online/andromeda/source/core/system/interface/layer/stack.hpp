#pragma once

#include "core/core.hpp"

#include "layer.hpp"

namespace Andromeda {
    class Layer::Stack {
      public:
        Stack() = default;
        ~Stack();

        void push_layer(Layer * layer);
        void pop_layer(Layer * layer);

        void push_overlay(Layer * overlay);
        void pop_overlay(Layer * overlay);

        std::vector<Layer *>::iterator begin() {
            return m_Layers.begin();
        }
        std::vector<Layer *>::iterator end() {
            return m_Layers.end();
        }
        std::vector<Layer *>::reverse_iterator rbegin() {
            return m_Layers.rbegin();
        }
        std::vector<Layer *>::reverse_iterator rend() {
            return m_Layers.rend();
        }

        std::vector<Layer *>::const_iterator begin() const {
            return m_Layers.begin();
        }
        std::vector<Layer *>::const_iterator end() const {
            return m_Layers.end();
        }
        std::vector<Layer *>::const_reverse_iterator rbegin() const {
            return m_Layers.rbegin();
        }
        std::vector<Layer *>::const_reverse_iterator rend() const {
            return m_Layers.rend();
        }

      private:
        std::vector<Layer *> m_Layers;
        unsigned int m_Layer_Insert_Index = 0;
    };
} /* Andromeda */
