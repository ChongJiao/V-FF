#include "glad/glad.h"
#include "Buffer.hpp"

namespace VFF{

    VertexBuffer::VertexBuffer(float* vertices, uint32_t size){
        glGenBuffers(1, &m_RenderID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    void VertexBuffer::Bind(){
        glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    }
    void VertexBuffer::UnBind(){
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    VertexBuffer::~VertexBuffer(){
        glDeleteBuffers(1, &m_RenderID);
    }

    IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count){
        glGenBuffers(1, &m_RenderID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }
    void IndexBuffer::Bind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
    }
    void IndexBuffer::UnBind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    IndexBuffer::~IndexBuffer(){
        glDeleteBuffers(1, &m_RenderID);
    }
}