#include "ImguiLayer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Application.hpp"

namespace VFF{
    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer"){

    }

    void ImGuiLayer::Attach(){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();(void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    // Enable Keyboard
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gameped 

        ImGui::StyleColorsDark();
        
        auto& app = Application::Get();
		ImGui_ImplGlfw_InitForOpenGL(app.GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410");

    }
    
    void ImGuiLayer::Detach(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        ImGui::ShowDemoWindow(&m_ShowDemo);
    }
	void ImGuiLayer::Update() {

	}

    void ImGuiLayer::Begin(){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

    }

    void ImGuiLayer::End(){
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}