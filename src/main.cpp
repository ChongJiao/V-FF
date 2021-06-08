#include <stdio.h>
#include "FFmpegLayer.hpp"
#include "Render.hpp"


int main(int argc, char** argv){
    VFF::Ref<Render> render = VFF::CreateRef<Render>(1080, 720);
    if(!render->Init("video-app")){
        printf("Render init error\n");
        return false;
    }

    Ref<Layer> fLayer = VFF::CreateRef<FFmpegLayer>(); 
    if(!((FFmpegLayer*)fLayer.get())->GetInitStatus()){
        printf("FFmpeg init error \n");
        return -1;
    }
    render->PushLayer(fLayer);
    
    render->Run();
    return 0;
}

