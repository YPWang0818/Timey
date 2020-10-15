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
			//SetVisibility(false);
		}


		ImGui::End();
		ImGui::PopStyleVar(2);

		RenderAllChildren();
	}


	MinimalViewWindow::~MinimalViewWindow()
	{
	}



	StandardlViewWindow::~StandardlViewWindow()
	{
		
	}


	void StandardlViewWindow::onUIRender()
	{
		


        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

       
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
       

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin(m_settings.Title.c_str(), nullptr, window_flags);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        else
		{
			TIMEY_CORE_ERROR("Dockspace Not configured."); 
		}

		ImGui::Text("This is a standard window.");


		AppContext& ctx = Application::getAppContext();

		if (ImGui::Button("Swith to minimal window."))
			ctx.currentWindowType = MainWindowType::minmal;
		
		ImGui::PopStyleVar(3);
   
		RenderAllChildren();

		ImGui::End();
	}
}