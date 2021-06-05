#pragma once
#include <string>
#include <stdint.h>
#include <vector>
namespace  VFF
{
    enum class ShaderDataType{
        None = 0, FLOAT, FLOAT2, FLOAT3, FLOAT4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t GetShaderDataTypeSize(ShaderDataType type){
        switch (type)
        {
        case ShaderDataType::FLOAT:
            return 4;
            break;
        case ShaderDataType::FLOAT2:
            return 4 * 2;
            break;
        case ShaderDataType::FLOAT3:
            return 4 * 3;
        case ShaderDataType::FLOAT4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            return 0;
            break;
        }
    }

    
    struct BufferElement{
        std::string m_Name;
        ShaderDataType m_Type;
        uint32_t m_Size;
        size_t m_Offset;
        bool m_Normalized;

        BufferElement(const ShaderDataType& type, const std::string& name, bool normalized = false)
        :m_Name(name), m_Type(type), m_Size(GetShaderDataTypeSize(type)), m_Offset(0), m_Normalized(normalized){} 

        BufferElement() = default;

        uint32_t GetCompomentCount() const{
            switch (m_Type)
            {
            case ShaderDataType::FLOAT:
                return 1;
            case ShaderDataType::FLOAT2:
                return 2;
            case ShaderDataType::FLOAT3:
                return 3;
            case ShaderDataType::FLOAT4:
                return 4;
            case ShaderDataType::Mat3:
                return 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4;
            case ShaderDataType::Int:
                return 1;
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Int3:
                return 3;
            case ShaderDataType::Int4:
                return 4;
            case ShaderDataType::Bool:
                return 1;
            default:
                printf("Unknow ShaderDataType\n");
                return -1;
            }
        }
        
    };
    class BufferLayout{
    public:
        BufferLayout(){}
        BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_Elements(elements)
        {
            CalculateElementStride();
        }
        inline uint32_t GetStride() const{return m_Stride;}
        inline const std::vector<BufferElement>& GetElements() const{return m_Elements;}
    private:
        void CalculateElementStride(){
            size_t offset = 0;
            m_Stride = 0;
            for(auto &element : m_Elements){
                element.m_Offset = offset;
                m_Stride += element.m_Size;
                offset += element.m_Size;
            }
        }
        
    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride;
    };
    
    class VertexBuffer {
    public:
        uint32_t m_RenderID;
        BufferLayout m_Layout;
    public:
        VertexBuffer(float* vertices, uint32_t size);
        virtual ~VertexBuffer();

        void Bind();
        void UnBind();

        void SetLayout(const BufferLayout& layout){m_Layout = layout;}
        const BufferLayout& GetLayout(){return m_Layout;}
    };
    class IndexBuffer{
    public:
        uint32_t m_RenderID;
        uint32_t m_Count;
    public:
        IndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~IndexBuffer();
        
        void Bind();
        void UnBind();

        uint32_t GetCount(){return m_Count;}
    };
} // namespace  VFF
