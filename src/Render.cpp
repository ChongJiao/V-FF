#pragma once
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
		glfwSwapInterval(1);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			printf("Fialed to initialized GLAD\n");
			return false;
		}

        // Set up Imgui platform
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();(void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    // Enable Keyboard
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gameped 

        ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        

        return true;
    }

    void Render::Run() {
		bool show_demo_window = false;
        while(!glfwWindowShouldClose(m_Window)){
            
            ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::ShowDemoWindow(&show_demo_window);
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				//ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}


            ImGui::Render();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            


            //Do what you want render
            for(auto& layer : m_LayerList){
                layer->Update();
            }


            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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