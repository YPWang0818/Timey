#include "timey_pch.h"
#include "imgui.h"
#include "UIWindows.h"
#include "Application.h"


namespace Timey {

	UIWindow::UIWindow(const  WindowUISettings& settings)
		:m_WindowID(settings.Title), m_settings(settings)
	{
	};



	Ref<UIWindow> UIWindow::getChildWindowByID(const TimeyID& ID)
	{
		return m_children_list[ID];
	}

	Ref<UIWindow> UIWindow::getChildWindowByName(const std::string& name)
	{
		return m_children_list[TIMEY_HASH_STR(name)];
	}

	void UIWindow::PushWindow(const Ref<UIWindow>& window)
	{
		m_children_list[window->getWindowID()] = window;
	}

	void UIWindow::onUpdate()
	{
		Begin();
		onUIRender();
		RenderAllChildren();
		End();
	}

	void UIWindow::RenderAllChildren()
	{
		for (auto& child : m_children_list) {
			if (child.second->IsVisible()) {
				child.second->onUpdate();
			}
		}
	}

	///////////////////////// Minimal Window ///////////////////////////

	MinimalViewWindow::MinimalViewWindow(const WindowUISettings& settings)
		:UIWindow(settings)
	{
		WindowUISettings sessionWinSet = {
			500, 800,
			"Add Session"
		};
		PushWindow(CreateRef<SessionViewWindow>(sessionWinSet));
		m_sessionWindowID = getChildWindowByName(sessionWinSet.Title)->getWindowID();
	};

	void MinimalViewWindow::onUIRender()
	{
		AppContext& ctx = Application::getAppContext();

		ImGui::Text("This is an minimal window.");
		if (ImGui::Button("Swith to standard window."))
		{
			ctx.currentWindowType = MainWindowType::standard;
		}

		if (ImGui::Button("Add new session"))
		{
			Ref<UIWindow> sessionWindow = getChildWindowByID(m_sessionWindowID);
			if (sessionWindow->IsVisible()) {
				sessionWindow->SetVisibility(false);
			}
			else {
				sessionWindow->SetVisibility(true);
			}
		}


	}

	void MinimalViewWindow::SetToCurrentWindow()
	{
		SetVisibility(true);
		AppContext& ctx = Application::getAppContext();
		WindowUISettings& settings = ctx.windowUISettings[(uint32_t)MainWindowType::minmal];
		ctx.mainWindow->setSize(settings.Width, settings.Hight);
		ctx.mainWindow->setTitle(settings.Title);

	}


	void MinimalViewWindow::Begin()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

		window_flags |= ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


		ImGui::Begin(m_settings.Title.c_str(), nullptr, window_flags);
		ImGui::PopStyleVar(2);
	}

	void MinimalViewWindow::End()
	{
		ImGui::End();
	}



	///////////////////////// Standard Window ///////////////////////////

	void StandardlViewWindow::Begin()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

		window_flags |= ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin(m_settings.Title.c_str(), nullptr, window_flags);
		ImGui::PopStyleVar(3);
	}

	void StandardlViewWindow::End()
	{
		ImGui::End();
	}


	void StandardlViewWindow::onUIRender()
	{
		AppContext& ctx = Application::getAppContext();


		ImGui::Text("This is an standard window.");

		if (ImGui::Button("Swith to minimal window."))
			ctx.currentWindowType = MainWindowType::minmal;


		ImGui::ShowDemoWindow();
		ImGui::ShowMetricsWindow();

	}

	void StandardlViewWindow::SetToCurrentWindow()
	{
		SetVisibility(true);
		AppContext& ctx = Application::getAppContext();
		WindowUISettings& settings = ctx.windowUISettings[(uint32_t)MainWindowType::standard];
		ctx.mainWindow->setSize(settings.Width, settings.Hight);
		ctx.mainWindow->setTitle(settings.Title);
	}



	///////////////////////// Session Window ///////////////////////////

	void SessionViewWindow::Begin()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

		window_flags |= ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


		ImGui::Begin(m_settings.Title.c_str(), nullptr, window_flags);
		ImGui::PopStyleVar(2);

	}
	void SessionViewWindow::End()
	{
		ImGui::End();
	}

	void SessionViewWindow::onUIRender()
	{

		

		ImGui::Text("This is a session view window.");

		tempSession.name.resize(maxTitleSize);
		ImGui::InputTextWithHint("Title","Session Title", &tempSession.name[0], maxTitleSize);
		ImGui::Text("Project: ");
		ImGui::Text("Date: "); ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15);
		ImGui::InputInt("", &year, 0, 0); //ImGui::SameLine();
		//ImGui::Text("/"); //ImGui::SameLine();
		//ImGui::InputInt("", &month, 1, 100); //ImGui::SameLine();
		//ImGui::Text("/"); //ImGui::SameLine();
		//ImGui::InputInt("", &day, 1, 100);


		if (ImGui::Button("Save")) {
			SetVisibility(false);
		}


	}
}
