#pragma once
#include "Common.hpp"
#include "RenderElement.hpp"

namespace VFF{
    class Layer{
    public:
        virtual void Update() = 0;
        virtual bool Init() = 0;
    public:
        Ref<VertexArray> m_DefaultObject;
        Ref<Texture> m_DefaultTexture;
        Ref<Shader> m_DefaultShader;
    };
}