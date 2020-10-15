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

		AppContext& ctx = Application::getAppContext();

		WindowUISettings minimalSettings = {
			ctx.mainWindows->getWidth(),
			ctx.mainWindows->getHight(),
			"Minamal Windows"
		};

		PushWindow(CreateRef<MinimalViewWindow>(minimalSettings));
	}

	void UILayer::OnDistory()
	{
		ImguiBackend::ShutDown();
	}

	void UILayer::OnEvent(Event& e)
	{
		ImguiBackend::OnEvent(e);
	}


	void UILayer::PushWindow(const Ref<UIWindow>& window)
	{
		m_WindowLists[window->getWindowID()] = window;
	}

	Ref<UIWindow> UILayer::getWindowByID(const TimeyID& ID)
	{
		return m_WindowLists[ID];
	}


	void UILayer::OnUpdate(float timestep)
	{
		glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImguiBackend::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = timestep;

	
		//ImGui::ShowMetricsWindow(&show_debug);
		//ImGui::ShowDemoWindow(&show_demo_window);

		for (auto& child : m_WindowLists) {
			if (child.second->IsVisible()) {
				child.second->onUIRender();
			}
		};


		ImGui::Render();
		ImguiBackend::DrawRenderData(ImGui::GetDrawData());
	
	}


}