#pragma once

#include "core/core.hpp"

#include "layer.hpp"

namespace Andromeda {
    class Layer::Stack {
      public:
        Stack() = default;
        ~Stack();

        void push_layer(std::shared_ptr<Layer> layer);
        void pop_layer(std::shared_ptr<Layer> layer);

        void push_overlay(std::shared_ptr<Layer> overlay);
        void pop_overlay(std::shared_ptr<Layer> overlay);

        std::vector<std::shared_ptr<Layer>>::iterator begin() noexcept;
        std::vector<std::shared_ptr<Layer>>::iterator end() noexcept;
        std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() noexcept;
        std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() noexcept;

        constexpr std::vector<std::shared_ptr<Layer>>::const_iterator begin() const noexcept;
        constexpr std::vector<std::shared_ptr<Layer>>::const_iterator end() const noexcept;
        constexpr std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rbegin() const noexcept;
        constexpr std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rend() const noexcept;

      private:
        std::vector<std::shared_ptr<Layer>> m_Layers;
        unsigned int m_Layer_Insert_Index = 0;
    };
} /* Andromeda */
