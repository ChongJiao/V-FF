#include <iostream>
#include "glad/glad.h"
#include "RenderElement.hpp"
#include "GLFW/glfw3.h"
#include <fstream>

namespace VFF{
	static GLenum GetOpenGLBasedType(const ShaderDataType& type) {
		switch (type)
		{
		case ShaderDataType::FLOAT:
			return GL_FLOAT;
		case ShaderDataType::FLOAT2:
			return GL_FLOAT;
		case ShaderDataType::FLOAT3:
			return GL_FLOAT;
		case ShaderDataType::FLOAT4:
			return GL_FLOAT;
		case ShaderDataType::Mat3:
			return GL_FLOAT;
		case ShaderDataType::Mat4:
			return GL_FLOAT;
		case ShaderDataType::Int:
			return GL_INT;
		case ShaderDataType::Int2:
			return GL_INT;
		case ShaderDataType::Int3:
			return GL_INT;
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		default:
			return 0;
		}
	}

    VertexArray::VertexArray(){
        glCreateVertexArrays(1, &m_RenderID);
    };
    void VertexArray::Bind(){
        glBindVertexArray(m_RenderID);
    } 
    void VertexArray::UnBind(){
        glBindVertexArray(0);
    }

    void VertexArray::DrawElement(){
        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
    }

    void VertexArray::SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer){

        if(vertexBuffer->GetLayout().GetElements().size() == 0){
            printf("Buffer has no layout \n");
            return;
        }       

        glBindVertexArray(m_RenderID);
        vertexBuffer->Bind();
        const auto& layout = vertexBuffer->GetLayout();
        
        uint32_t vertexBufferIndex = 0;
        for(auto& element : layout.GetElements()){

            glVertexAttribPointer(vertexBufferIndex, 
            element.GetCompomentCount(), 
            GetOpenGLBasedType(element.m_Type),
            element.m_Normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            (const void*)(element.m_Offset));

			glEnableVertexAttribArray(vertexBufferIndex);

            vertexBufferIndex ++;
        }

        m_VertexBuffer = vertexBuffer;
    }

	void VertexArray::SetIndexBuffer(Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RenderID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

    //TextureObject
    Texture::Texture() 
    {
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void Texture::Init(const char* texturePath){
        //TODO: Set texture From Texture path
    }
    void Texture::Init(const int& width, const int& height){
        m_TextureWidth = width;
        m_TextureHeight = height;
        m_InternalFormat = GL_RGB;
        m_DataFormat = GL_RGB;
    }
    void Texture::Init(const int& width, const int& height, const int& internal_format, const int& data_format)
    {
        m_TextureWidth = width;
        m_TextureHeight = height;
        m_InternalFormat = internal_format;
        m_DataFormat = data_format;
    }

    void Texture::SetTextureBuffer(uint8_t* data_buffer){
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_TextureWidth, m_TextureHeight, 0, m_DataFormat, GL_UNSIGNED_BYTE, data_buffer);
    }
    
    void Texture::BindTexture(){
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void Shader::AttachShader(const std::string& vertex_shader_string, const std::string& fragement_shader_string){
        unsigned int vertex, fragement;

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        const char* vertex_shader_source_cstr = vertex_shader_string.c_str();
        glShaderSource(vertex, 1, &vertex_shader_source_cstr, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");

        fragement = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragement_shader_source_cstr = fragement_shader_string.c_str();
        glShaderSource(fragement, 1, &fragement_shader_source_cstr, NULL);
        glCompileShader(fragement);
        CheckCompileErrors(fragement, "FRAGEMENT");

        //shader program
        m_Program_ID = glCreateProgram();
        glAttachShader(m_Program_ID, vertex);
        glAttachShader(m_Program_ID, fragement);

        glLinkProgram(m_Program_ID);
        CheckCompileErrors(m_Program_ID, "PROGRAM");
        glDeleteShader(vertex);
        glDeleteShader(fragement);       
    }

    //Shader Object init from shader string and fragment string
    Shader::Shader(const std::string& name, const std::string& vertex_shader_string, 
    const std::string& fragement_shader_string): m_ShaderName(name){
        AttachShader(vertex_shader_string, fragement_shader_string);
    }

    Shader::Shader(const std::string& name, const std::string& shader_file) : m_ShaderName(name){
        // TODO Get Shader content form shader file 
        std::ifstream fs;
        fs.open(shader_file, std::ios::binary | std::ios::in);
        if(!fs){
            printf("shader file open error");
            return;
        }
        fs.seekg(0, std::ios::end);
        size_t len = fs.tellg();
        fs.seekg(0, std::ios::beg);
        std::string buffer;
        if(len != -1){
            buffer.resize(len);
            fs.read(&buffer[0], len);
            fs.close();
            // TODO: parse vertex shader and fragement shader from buffer 

            const char* token = "#type";
            size_t token_length = strlen(token);
            size_t token_pos = buffer.find(token, 0);
            if(token_pos == std::string::npos)return;
            size_t el = buffer.find_first_of("\r\n", token_pos);
            if(el == std::string::npos)return;
            size_t begin = token_pos + token_length + 1;
            std::string type = buffer.substr(begin, el - begin);
            if(type != "vertex")return;
            
            size_t shader_content_line = buffer.find_first_not_of("\r\n", el);
            if(shader_content_line == std::string::npos)return;
            size_t next_token_pos = buffer.find(token, shader_content_line);
            if(next_token_pos == std::string::npos)return;
            
            std::string vertex_shader_str, fragement_shader_str;
            vertex_shader_str = buffer.substr(shader_content_line, next_token_pos - shader_content_line);

            begin = next_token_pos + token_length + 1;
            el = buffer.find_first_of("\r\n", next_token_pos);
            type = buffer.substr(begin, el - begin);
            if(type != "fragment")return;
            shader_content_line = buffer.find_first_not_of("\r\n", el);
            if(shader_content_line == std::string::npos)return;
            fragement_shader_str = buffer.substr(shader_content_line, buffer.size() - shader_content_line);
            
            AttachShader(vertex_shader_str, fragement_shader_str);
  
        }else {
            return;
        }

    }

    const char* Shader::LoadShaderString(const std::string& shader_file){
        return "test";
    }

    void Shader::Use(){
        glUseProgram(m_Program_ID);
    }


    void Shader::setBool(const std::string& name, bool value) const{
        glUniform1i(glGetUniformLocation(m_Program_ID, name.c_str()), value);
    }

    void Shader::setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_Program_ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(m_Program_ID, name.c_str()), value);
    }

    void Shader::CheckCompileErrors(GLuint shader, std::string type){
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                printf("Error::Shader_Compile_Compile_Error of type %s, and log is %s \n", type, infoLog);
            }
        }else{
            glGetShaderiv(shader, GL_LINK_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                printf("Error::Shader_Linked_error of type %s, and log is %s \n", type, infoLog);
            }
        }
    }

}
