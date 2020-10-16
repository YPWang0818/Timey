#include "timey_pch.h"
#include "imgui.h"
#include "UIWindows.h"
#include "Application.h"


namespace Timey {

	UIWindow::UIWindow(const  WindowUISettings& settings)
		:m_WindowID(settings.Title), m_settings(settings)
	{
		SetVisibility(true);
	};



	void UIWindow::PushWindow(const Ref<UIWindow>& window)
	{
		m_children_list.push_back(window);
	}

	void UIWindow::RenderAllChildren()
	{
		for (auto& child : m_children_list) {
			if (child->IsVisible()) {
				child->onUIRender();
			}
		}

	}






	void MinimalViewWindow::onUIRender()
	{
		ImGuiWindowFlags window_flags = 0;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;


		ImGui::Begin(m_settings.Title.c_str(), nullptr, window_flags);
		ImGui::Text("This is an minimal window.");

	
		AppContext& ctx = Application::getAppContext();

		if (ImGui::Button("Swith to standard window."))
		{
			ctx.currentWindowType = MainWindowType::standard;
		}


		ImGui::End();
		ImGui::PopStyleVar(2);

		RenderAllChildren();
	}

	void MinimalViewWindow::SetToCurrent()
	{
		SetVisibility(true);
		AppContext& ctx = Application::getAppContext();
		WindowUISettings& settings = ctx.windowUISettings[(uint32_t)MainWindowType::minmal];
		
		ctx.mainWindow->setSize(settings.Width, settings.Hight);
		ctx.mainWindow->setTitle(settings.Title);
		

	}


	MinimalViewWindow::~MinimalViewWindow()
	{
	}



	StandardlViewWindow::~StandardlViewWindow()
	{
		
	}


	void StandardlViewWindow::onUIRender()
	{
		

		ImGuiWindowFlags window_flags =  ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Standard Window", nullptr, window_flags);
		ImGui::PopStyleVar(3);
	

		ImGui::Text("This is an minimal window.");

		AppContext& ctx = Application::getAppContext();

		if (ImGui::Button("Swith to minimal window."))
			ctx.currentWindowType = MainWindowType::minmal;
	
   
		RenderAllChildren();

		ImGui::End();
	}
	void StandardlViewWindow::SetToCurrent()
	{
		SetVisibility(true);
		AppContext& ctx = Application::getAppContext();
		WindowUISettings& settings = ctx.windowUISettings[(uint32_t)MainWindowType::standard];

		ctx.mainWindow->setSize(settings.Width, settings.Hight);
		ctx.mainWindow->setTitle(settings.Title);
	}
}