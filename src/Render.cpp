#pragma once
#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Render.hpp"
namespace VFF
{
	Render::Render(GLFWwindow * windowHandle)
	{
		m_Window = windowHandle;
	}
	Render::Render(int width, int height)
    : m_RenderWidth(width), m_RenderHeight(height)
    {
    }

    bool Render::Init(const char* title){
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
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			printf("Fialed to initialized GLAD\n");
			return false;
		}

        return true;
    }

    void Render::Run() {
        while(!glfwWindowShouldClose(m_Window)){

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Do what you want render
            for(auto& layer : m_LayerList){
                layer->Update();
            }

            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }
    }

    void Render::PushLayer(Ref<Layer> layer){
        m_LayerList.push_back(layer);
    }

	void Render::PopLayer()
	{
        m_LayerList.pop_back();
	}



    Render::~Render(){
        while(!m_LayerList.empty()){
            m_LayerList.pop_back();
        }
    }
}