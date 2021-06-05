#pragma once
#include <stdint.h>
struct VideoState;
struct AVFrame;
enum AVPixelFormat;

class VideoReader{
public:
    VideoReader();


    bool VideoOpen(const char* filename);
    /*
    Read Raw Frame Data from Video state dict, the data should be the YUV data
    */
    bool VideoFrameDataReader(uint8_t* buffer_data, bool use_rgb_convert=false);

    void VideoClose();

    int GetVideoFrameWidth();
    int GetVideoFrameHeight();
    
    virtual ~VideoReader();

	void SaveGrayFrame(uint8_t* buffer, const char* filename, int width, int height);
	void SaveRGBFrame(uint8_t* buffer, const char* filename, int width, int height);

private:
    void ConvertYUV420P2RGB(uint8_t* buffer_data, const int& width, const int& height);

    void ConvertStorageFormat(AVPixelFormat& type, AVFrame* frame, uint8_t* buffer_data);

    void ConvertYUVJ420PFormat2CompFormat(AVFrame* frame, uint8_t* buffer_data);

    VideoState* m_VideoState;

};