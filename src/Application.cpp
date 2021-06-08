#pragma once
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Application.hpp"

namespace VFF
{
	Application* Application::m_Instance = nullptr;
	Application::Application()
	{
        m_Instance = this;
        m_ImGuiLayer = CreateRef<ImGuiLayer>();
        m_LayerList.push_back(m_ImGuiLayer);

	}

    bool Application::Init(const char* title, const int& width, const int& height){
        m_RenderWidth = width;
        m_RenderHeight = height;
        if(!glfwInit()){
            printf("Init Render Error\n");
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        m_Window = glfwCreateWindow(m_RenderWidth, m_RenderHeight, title, NULL, NULL);
        if(!m_Window){
            printf("Create Window error \n");
            return false;
        }
        glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			printf("Fialed to initialized GLAD\n");
			return false;
		}

        // TODO Attach All Layer in Layerlist;
        
        for(auto& layer : m_LayerList){
            layer->Attach();
        }
        
        return true;
    }

    void Application::Run() {
		bool show_demo_window = false;
        while(!glfwWindowShouldClose(m_Window)){
            
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            

            //Do what you want render

            for(auto& layer : m_LayerList){
                layer->Update();
            }

            m_ImGuiLayer->Begin();
            for(auto& layer : m_LayerList){
                layer->ImGuiRender();
            }
            m_ImGuiLayer->End();


			glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }
    }

    void Application::PushLayer(Ref<Layer> layer){
        m_LayerList.push_back(layer);
    }

	void Application::PopLayer()
	{
        m_LayerList.pop_back();
	}



    Application::~Application(){
        while(!m_LayerList.empty()){
            auto topLayer = m_LayerList.front();
            topLayer->Detach();
            m_LayerList.pop_back();
        }

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}