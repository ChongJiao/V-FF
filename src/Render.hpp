#pragma once
#include "Layer.hpp"
#include <stack>
#include <list>
#include "common.hpp"
struct GLFWwindow;
namespace VFF {
    class Render{
    
    public:
		Render(GLFWwindow* windowHandle);
        Render(int width, int height);
        ~Render();


        bool Init(const char* title);
        void Run();

        void PushLayer(Ref<Layer> layer);
        void PopLayer();

        GLFWwindow* GetNativeWindow(){
            return m_Window;
        }
        //void RemoveLayer(int layerIndex);
    private:
        GLFWwindow* m_Window; 
        int m_RenderWidth;
        int m_RenderHeight;   
    
        std::list<Ref<Layer>> m_LayerList;  

    };
}