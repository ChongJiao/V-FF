#pragma once
#include "ImguiLayer.hpp"
#include <stack>
#include <list>
#include "common.hpp"
struct GLFWwindow;
namespace VFF {
    class Application{
    
    public:
		Application();
        virtual~ Application();


        bool Init(const char* title, const int& width, const int& height);
        void Run();

        void PushLayer(Ref<Layer> layer);
        void PopLayer();

        inline static Application& Get(){return *m_Instance;} 
        GLFWwindow* GetNativeWindow(){
            return m_Window;
        }
        //void RemoveLayer(int layerIndex);
    private:
        static Application* m_Instance;
        GLFWwindow* m_Window; 
        int m_RenderWidth;
        int m_RenderHeight;   
        
        Ref<ImGuiLayer> m_ImGuiLayer;
        std::list<Ref<Layer>> m_LayerList;  

    };
}