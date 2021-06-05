#pragma once
#include "Layer.hpp"
#include "video_reader.hpp"

using namespace VFF;
class FFmpegLayer : public Layer{
public:
    FFmpegLayer(){};
    virtual ~FFmpegLayer(){
        if(!m_FrameBuffer) delete m_FrameBuffer; 
    }

    virtual void Update() override;
    virtual bool Init() override;

    bool OpenVideoByFFmpge(const char* filename);

private:
    void SetShowProps(const int& width, const int& height);
    
    Ref<VideoReader> m_VideoReader;
    int m_FrameWidth;
    int m_FrameHeight;
    uint8_t* m_FrameBuffer;
};