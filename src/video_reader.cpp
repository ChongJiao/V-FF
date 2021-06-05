extern "C"
{
#include "libavcodec/codec.h"
#include "libavcodec/packet.h"
#include "libavformat/avformat.h"
#include "libavutil/frame.h"
};

#include "video_reader.hpp"
struct VideoState {
	AVCodec* av_video_codec;
	AVFormatContext* av_format_ctx = NULL;
	AVCodecContext* av_video_codec_ctx;
	AVFrame* av_video_frame;
	AVPacket* av_video_packet;

	int video_frame_width, video_frame_height;
	int video_stream_index;

	AVRational time_base;
};

void VideoReader::SaveGrayFrame(uint8_t* buffer, const char* filename, int width, int height) {
	FILE* f;
	f = fopen(filename, "w");
	fprintf(f, "P5\n%d %d\n%d\n", width, height, 255);
	for (int i = 0; i < height; ++i)
		fwrite(buffer + i * width, 1, width, f);
	fclose(f);
}
void VideoReader::SaveRGBFrame(uint8_t* buffer, const char* filename, int width, int height) {
	FILE* f;
	f = fopen(filename, "w");
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
	int i, j;
	for (j = 0; j < height; ++j) {
		for (i = 0; i < width; ++i) {
			auto R = buffer[(j * width + i) * 3];
			auto G = buffer[(j * width + i) * 3 + 1];
			auto B = buffer[(j * width + i) * 3 + 2];
			fprintf(f, "%d %d %d ", R, G, B);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}




VideoReader::VideoReader(){
    m_VideoState = new VideoState();
}
bool VideoReader::VideoOpen(const char* filename){
    auto& av_format_ctx = m_VideoState->av_format_ctx;
    auto& av_video_codec_ctx = m_VideoState->av_video_codec_ctx;
    auto& av_video_codec = m_VideoState->av_video_codec;
    auto& av_video_frame = m_VideoState->av_video_frame;
    auto& av_video_packet = m_VideoState->av_video_packet;

    auto& frame_width = m_VideoState->video_frame_width;
    auto& frame_height = m_VideoState->video_frame_height;
    auto& video_stream_index = m_VideoState->video_stream_index;
    auto& time_base = m_VideoState->time_base;

    // setup avformat input   
	av_format_ctx = avformat_alloc_context();
	if (!av_format_ctx)
	{
		printf("allocate the av_format_ctx error \n");
		return false;
	}
	auto errorCode = avformat_open_input(&av_format_ctx, filename, NULL, NULL);
	if(errorCode < 0){
		char log[1024];
		av_strerror(errorCode, log, 1024);
        printf("Avformat open from file error is %s \n", log);
        return false;
    }

    //find steam info and set codec 
    if(avformat_find_stream_info(av_format_ctx, NULL) < 0) 
    {
        printf("Avformate find stream error \n");
        return false;
    }

    video_stream_index = -1;
    AVCodecParameters* video_codec_parameters;
    for(auto i = 0; i < av_format_ctx->nb_streams; ++ i){
        video_codec_parameters = av_format_ctx->streams[i]->codecpar;
        av_video_codec = avcodec_find_decoder(video_codec_parameters->codec_id);
        if(!av_video_codec){
            continue;
        }

        if(video_codec_parameters->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_index = i;
            frame_width = video_codec_parameters->width;
            frame_height = video_codec_parameters->height;
            time_base = av_format_ctx->streams[i]->time_base;
            break;
        }
    }
    if(video_stream_index == -1){
        printf("find video stream info error \n");
        return false;
    }


    //initial av_codec_contxt;
    av_video_codec_ctx = avcodec_alloc_context3(av_video_codec);
    if(!av_video_codec_ctx){
        printf("allocate the av_codec_ctx failed \n");
        return false;
    }
    if(avcodec_parameters_to_context(av_video_codec_ctx, video_codec_parameters) < 0 ){
        printf("copy codec_parameters to codec_contxt \n");
        return false;
    }
    if(avcodec_open2(av_video_codec_ctx, av_video_codec, NULL) < 0){
        printf("initilized the av_codec_contxt error \n");
        return false;
    }

    av_video_packet = av_packet_alloc();
    if(!av_video_packet){
        printf("allocate the packet error");
        return false;
    }
    av_video_frame = av_frame_alloc();
    if(!av_video_frame){
        printf("allocate the frame error");
        return false;
    }

    return true;

}
int VideoReader::GetVideoFrameWidth(){
    return m_VideoState->video_frame_width;
}
int VideoReader::GetVideoFrameHeight(){
    return m_VideoState->video_frame_height;
}

void VideoReader::ConvertYUVJ420PFormat2CompFormat(AVFrame* frame, uint8_t* buffer_data){
    int width = frame->width;
    int height = frame->height;
    
    int i , j;
    for(j = 0; j < height; ++ j){
        for(i = 0; i < width; ++ i){
            buffer_data[(j * width + i) * 3] = frame->data[0][j * width + i];
            buffer_data[(j * width + i) * 3 + 1] = frame->data[1][((j / 2) * (width / 2) + i / 2)];
            buffer_data[(j * width + i) * 3 + 2] = frame->data[2][((j /2 ) * (width / 2)+ i / 2)];
        }
    }

}
void VideoReader::ConvertStorageFormat(AVPixelFormat& type, AVFrame* frame, uint8_t* data_buffer){
    switch (type)
    {
    case AVPixelFormat::AV_PIX_FMT_YUVJ420P:
        ConvertYUVJ420PFormat2CompFormat(frame, data_buffer);      
        break;
    default:
        break;
    }
}

void VideoReader::ConvertYUV420P2RGB(uint8_t* buffer_data, const int& width, const int& height){
    int i, j;
    for(j = 0; j < height; ++ j) {
        for(i = 0; i < width; ++ i) {
            auto& Y = buffer_data[(j * width + i) * 3];
            auto& U = buffer_data[(j * width + i) * 3 + 1];
            auto& V = buffer_data[(j * width + i) * 3 + 2];

            auto R = 1.164 * (Y - 16) + 1.596 * (V - 128);
            auto G = 1.164 * (Y - 16) - 0.813 * (V - 128) - 0.391 * (U - 128);
            auto B = 1.164 * (Y - 16) + 2.018 * (U - 128);

            R = R > 255 ? 255 : R;
            G = G > 255 ? 255 : G;
            B = B > 255 ? 255 : B;
            R = R < 0 ? 0 : R;
            G = G < 0 ? 0 : G;
            B = B < 0 ? 0 : B;
            
            Y = R;
            U = G;
            V = B;
        }
    }
}


bool VideoReader::VideoFrameDataReader(uint8_t* buffer_data, bool use_rgb_convert){
    auto& av_format_ctx = m_VideoState->av_format_ctx;
    auto& av_video_codec_ctx = m_VideoState->av_video_codec_ctx;
    auto& av_video_codec = m_VideoState->av_video_codec;
    auto& av_video_frame = m_VideoState->av_video_frame;
    auto& av_video_packet = m_VideoState->av_video_packet;

    auto& frame_width = m_VideoState->video_frame_width;
    auto& frame_height = m_VideoState->video_frame_height;
    auto& video_stream_index = m_VideoState->video_stream_index;
    auto& time_base = m_VideoState->time_base;

    int response = -1;
    while(av_read_frame(av_format_ctx, av_video_packet) >= 0) {
        if(av_video_packet->stream_index != video_stream_index) {
            av_packet_unref(av_video_packet);
            continue;
        }      
        
        response = avcodec_send_packet(av_video_codec_ctx, av_video_packet);
        if(response < 0){
            printf("error in send packet from avcodec contxt\n");
            return false;
        }
        response = avcodec_receive_frame(av_video_codec_ctx, av_video_frame);
        if(response == AVERROR(EAGAIN) || response == AVERROR(EOF)){
            av_packet_unref(av_video_packet);
            continue;
        }else if(response < 0){
            printf("error in recevie frame \n");
            return false;
        }
        av_packet_unref(av_video_packet);
        break;
    }

    av_video_codec_ctx->pix_fmt;

    ConvertStorageFormat(av_video_codec_ctx->pix_fmt, av_video_frame, buffer_data);
    
    // convert data to RGB-format in cpu 
    if(use_rgb_convert){
        ConvertYUV420P2RGB(buffer_data, frame_width, frame_height);
    }
    return true;    
}

void VideoReader::VideoClose(){
    avformat_close_input(&m_VideoState->av_format_ctx);
    avformat_free_context(m_VideoState->av_format_ctx);
    avcodec_free_context(&m_VideoState->av_video_codec_ctx);
    av_frame_free(&m_VideoState->av_video_frame);
    av_packet_free(&m_VideoState->av_video_packet);
}
VideoReader::~VideoReader(){
    if(m_VideoState != NULL){
        delete m_VideoState;
    }
}