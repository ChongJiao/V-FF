#pragma once
#include <stdint.h>
#include <string>
#include "Common.hpp"
#include <vector>
#include "Buffer.hpp"

typedef unsigned int GLuint;
namespace VFF{
    // VertexArray
    class VertexArray{
    public:
        VertexArray();

        void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
        void SetIndexBuffer(Ref<IndexBuffer>& indexBuffer);       

        void Bind();
        void UnBind();

        void DrawElement();
    private:
        unsigned int m_RenderID;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };

    class Texture{
    public:
        Texture();
		virtual ~Texture() {};

        //TODO: Load Texture From Image File
        void Init(const char* texturePath);

        void Init(const int& width, const int& height);
        void Init(const int& width, const int& height, const int& internal_format, const int& data_format);
        
        // Set From Video prop
        void SetTextureBuffer(uint8_t* data);

        
        void BindTexture();

        GLuint m_TextureID;

    private:
        int m_TextureWidth, m_TextureHeight, m_InternalFormat, m_DataFormat;

    };

    class Shader{
    public:
        unsigned int m_Program_ID;
        Shader(const char* vertex_shader_string, const char* fragment_shader_string);
        //TODO: init shader from shader file
        //Shader(const char* vertex_shader_file, const char* fragment_shader_file);

        void Use();


        //Set Value In Shader Program
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name ,float value) const;
    
        //TODO: set vec and mat by glm
    private:
        void CheckCompileErrors(GLuint shader, std::string type);
    };
}