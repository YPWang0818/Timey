#include "timey_pch.h"
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

		PushWindow(CreateRef<MinimalViewWindow>(ctx.windowUISettings[(uint32_t)MainWindowType::minmal]));
		PushWindow(CreateRef<StandardlViewWindow>(ctx.windowUISettings[(uint32_t)MainWindowType::standard]));

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

	Ref<UIWindow> UILayer::getWindowByName(const std::string& name)
	{
		return m_WindowLists[TIMEY_HASH_STR(name)];
	}


	void UILayer::OnUpdate(float timestep)
	{

		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = timestep;

		ImguiBackend::NewFrame();

		for (auto& child : m_WindowLists) {
			if (child.second->IsVisible()) {
				child.second->onUpdate();
			}
		};

		ImGui::Render();
		ImguiBackend::DrawRenderData(ImGui::GetDrawData());
	
	}


}