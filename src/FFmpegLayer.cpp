#include "FFmpegLayer.hpp"
#include "GLFW/glfw3.h"

void FFmpegLayer::Attach(){

}
void FFmpegLayer::Detach(){

}

FFmpegLayer::FFmpegLayer(): Layer("FFmpegLayer"){
 
    const char* vertex_shader_string =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
	"out vec3 ourColor;\n"
	"out vec2 texCoord;\n"
    "void main()\n"
    "{\n"
    "    gl_Position=vec4(aPos, 1.0);\n"
    "    texCoord = aTexCoord;\n"
	"    ourColor = aColor;\n"
    "}\n";
    const char* fragment_shader_string = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 texCoord;\n"
	"in vec3 ourColor;\n"
    "uniform sampler2D image;\n"
    "void main()\n"
    "{\n"
    "    vec4 texData = texture(image, texCoord);\n"
    "    float Y = texData.x;\n"
    "    float U = texData.y;\n"
    "    float V = texData.z;\n"
    "    float R = 1.164 * (Y - 0.0625) + 1.596 * (V - 0.5);\n"
    "    float G = 1.164 * (Y - 0.0625) - 0.813 * (V - 0.5) - 0.391 * (U - 0.5);\n"
    "    float B = 1.164 * (Y - 0.0625) + 2.018 * (U - 0.5);\n"
    "    FragColor = vec4(R, G, B, 0.5f);\n"
    "}\n"
    ;

    m_DefaultShader = VFF::CreateRef<Shader>(vertex_shader_string, fragment_shader_string);
	m_DefaultShader->Use();
	m_DefaultShader->setInt("image", 0);


    float vertex_data[] = {
        //position-color-texturecoord
        1.f,   1.f, 0.0f, 1.f, 0.f, 0.f, 1.0f, 0.0f,
        1.f,  -1.f, 0.0f, 1.f, 0.f, 0.f, 1.0f, 1.0f,
        -1.f, -1.f, 0.0f, 1.f, 0.f, 0.f, 0.0f, 1.0f,
        -1.f,  1.f, 0.0f, 1.f, 0.f, 0.f, 0.0f, 0.0f,
    };
    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

	m_DefaultObject = CreateRef<VertexArray>();

    VFF::Ref<VertexBuffer> vbf = VFF::CreateRef<VertexBuffer>(vertex_data, sizeof(vertex_data));
    VFF::Ref<IndexBuffer> ibf = VFF::CreateRef<IndexBuffer>(indices, 6);  

    vbf->SetLayout({
		{ShaderDataType::FLOAT3, "aPos"},
		{ShaderDataType::FLOAT3, "aColor"},
        {ShaderDataType::FLOAT2, "aTexCoord"}
    });  
	m_DefaultObject->SetVertexBuffer(vbf);
	m_DefaultObject->SetIndexBuffer(ibf);


	m_DefaultTexture = CreateRef<Texture>();

    // OpenVideo
    if(!OpenVideoByFFmpge("D:\\work\\cpp\\video-app\\src\\Test2.mp4")){
        printf("Open Video Error \n");
        return;
    }
    m_InitialStatus = true;
}

void FFmpegLayer::SetShowProps(const int& width, const int& height){
    m_DefaultTexture->Init(width, height);  
}

bool FFmpegLayer::OpenVideoByFFmpge(const char* filename){
    m_VideoReader = VFF::CreateRef<VideoReader>();
    if(m_VideoReader->VideoOpen(filename)){
        m_FrameWidth = m_VideoReader->GetVideoFrameWidth();
        m_FrameHeight = m_VideoReader->GetVideoFrameHeight();
        m_FrameBuffer = new uint8_t[m_FrameWidth * m_FrameHeight * 3];
        SetShowProps(m_FrameWidth, m_FrameHeight);
        return true;
    }
    else {
        printf("Open Video Error \n");
        return false;
    } 
}
    
void FFmpegLayer::Update() {

    if(!m_VideoReader->VideoFrameDataReader(m_FrameBuffer)){
        printf("can not get vide data");
        return;
    }

    m_DefaultTexture->BindTexture();
    m_DefaultTexture->SetTextureBuffer(m_FrameBuffer);
    m_DefaultShader->Use();
    m_DefaultObject->Bind();
    m_DefaultObject->DrawElement();
}
