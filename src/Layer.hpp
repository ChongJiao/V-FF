#pragma once
#include "Common.hpp"
#include "RenderElement.hpp"

namespace VFF{
    class Layer{
    public:
        Layer(const std::string& name = "Layer");

        virtual void Attach() = 0;
        virtual void Detach() = 0;

		virtual void Update() {};

		virtual void ImGuiRender() {};
    public:
        std::string m_LayerName;

    };
}