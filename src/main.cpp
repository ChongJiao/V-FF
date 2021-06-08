#include <stdio.h>
#include "FFmpegLayer.hpp"
#include "Application.hpp"

int main(int argc, char** argv){
    VFF::Ref<Application> app = VFF::CreateRef<Application>();
    if(!app->Init("video-app", 1280, 720)){
        printf("Render init error\n");
        return false;
    }

    Ref<Layer> fLayer = VFF::CreateRef<FFmpegLayer>(); 
    if(!((FFmpegLayer*)fLayer.get())->GetInitStatus()){
        printf("FFmpeg init error \n");
        return -1;
    }
    app->PushLayer(fLayer);
    
    app->Run();
    return 0;
}

