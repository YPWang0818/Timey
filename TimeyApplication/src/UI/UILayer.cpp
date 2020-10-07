#include "timey_pch.h"
#include "glad/glad.h"
#include "UILayer.h"
#include "imgui.h"
#include "ImguiBackend.h"
#include "Application.h"

namespace Timey {


	void UILayer::OnInit()
	{
		ImguiBackend::Init();
		ImGui::StyleColorsDark();

		Application& app = Application::getApplication();
		WindowSettings settings = {
			app.getWindowHandle()->getWidth(),
			app.getWindowHandle()->getHight(),
			"Minimal Window" };

		PushWindow(CreateRef<MinimalViewWindow>(settings));

	}

	void UILayer::OnDistory()
	{
		ImguiBackend::ShutDown();
	}

	void UILayer::OnEvent(Event& e)
	{
		ImguiBackend::OnEvent(e);
	}


	void UILayer::PushWindow(const Ref<Window>& window)
	{
		m_window_list.push_back(window);
	}


	void UILayer::OnUpdate(float timestep)
	{
		glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImguiBackend::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = timestep;
		bool show_demo_window = false;
		bool show_debug = false;

	


		//ImGui::ShowMetricsWindow(&show_debug);
		//ImGui::ShowDemoWindow(&show_demo_window);

		

		for (auto& child : m_window_list) {
			if (child->IsVisible()) {
				child->onUIRender();
			}
		};


		ImGui::Render();
		ImguiBackend::DrawRenderData(ImGui::GetDrawData());
	
	}


}