#pragma once
#include "Layer.hpp"
#include "video_reader.hpp"

using namespace VFF;
class FFmpegLayer : public Layer{
public:
    FFmpegLayer();
    virtual ~FFmpegLayer(){
        if(!m_FrameBuffer) delete m_FrameBuffer; 
    }

    virtual void Attach() override;
    virtual void Detach() override;
    virtual void Update() override;

    bool OpenVideoByFFmpge(const char* filename);
    inline bool GetInitStatus() {return m_InitialStatus;}

private:
    void SetShowProps(const int& width, const int& height);
    
    Ref<VideoReader> m_VideoReader;
    int m_FrameWidth;
    int m_FrameHeight;
    uint8_t* m_FrameBuffer;

    Ref<VertexArray> m_DefaultObject;
    Ref<Texture> m_DefaultTexture;
    Ref<Shader> m_DefaultShader;
    
    bool m_InitialStatus = false;
};