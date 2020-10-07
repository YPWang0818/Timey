#include "timey_pch.h"
#include "imgui.h"
#include "Windows.h"


namespace Timey {

	Window::Window(const WindowSettings& settings)
		:m_settings(settings)
	{
		SetVisibility(true);
	};



	void Window::PushWindow(const Ref<Window>& window)
	{
		m_children_list.push_back(window);
	}

	void Window::RenderAllChildren()
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

	
		if (ImGui::Button("Show Standard Window"))
			m_clicked++;
		if (m_clicked & 1)
		{
			ImGui::SameLine();
			ImGui::Text("Thanks for clicking me!");
		}

		ImGui::End();
		ImGui::PopStyleVar(2);

		RenderAllChildren();
	}



	MinimalViewWindow::~MinimalViewWindow()
	{
	}
}