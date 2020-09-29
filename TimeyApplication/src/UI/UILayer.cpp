#include "timey_pch.h"
#include "glad/glad.h"
#include "UILayer.h"
#include "imgui.h"
#include "ImguiBackend.h"

namespace Timey {


	void UILayer::OnInit()
	{
		ImguiBackend::Init();
		ImGui::StyleColorsDark();
	}

	void UILayer::OnDistory()
	{
		ImguiBackend::ShutDown();
	}

	void UILayer::OnEvent(Event& e)
	{
		ImguiBackend::OnEvent(e);
	}

	void UILayer::OnUpdate(float timestep)
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImguiBackend::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = timestep;
		bool show_demo_window = false;
		bool show_debug = true;


		ImGui::Begin("Hellow World.");
		ImGui::Text("This is some useful text.");
		ImGui::End();

		ImGui::ShowMetricsWindow(&show_debug);
		ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::Render();
		ImguiBackend::DrawRenderData(ImGui::GetDrawData());
	
	}

}