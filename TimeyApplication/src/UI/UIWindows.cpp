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

	#define UGE_STRINGIFY(x) #x
	#define UGE_TOSTRING(x) UGE_STRINGIFY(x)
	#define DUMMY_LABLE "##" UGE_TOSTRING(__COUNTER__)
	// ImGui Widgets need a unique name for every widgets. This is a temprary hack. 

	void SessionViewWindow::onUIRender()
	{
		
		
		ImGui::Text("This is a session view window.");
		ImGui::InputTextWithHint("Title","Session Title", &m_title[0], maxTitleSize);
		ImGui::Text("Project: ");
		ImGui::Text("Date (yyyy:mm::dd) "); ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15);

		ImGui::InputInt(":" DUMMY_LABLE, (int*)&m_startTime.date.year, 0, 0); ImGui::SameLine();
		ImGui::DragInt(":" DUMMY_LABLE, (int*)&m_startTime.date.month, scrlspd, 1, 12);
		ImGui::SameLine();
		ImGui::DragInt(DUMMY_LABLE, (int*)&m_startTime.date.day, scrlspd, 1, 31);

		ImGui::Text("Time (hh:mm:ss) "); 
		ImGui::InputInt(":" DUMMY_LABLE, (int*)&m_startTime.time.hour, 0, 0);
		ImGui::SameLine();
		ImGui::DragInt(":" DUMMY_LABLE, (int*)&m_startTime.time.minute, scrlspd, 1, 60);
		ImGui::SameLine();
		ImGui::DragInt(DUMMY_LABLE, (int*)&m_startTime.time.second, scrlspd, 1, 60);
		ImGui::SameLine();
		ImGui::Text("-");
		ImGui::InputInt(":" DUMMY_LABLE, (int*)&m_endTime.time.hour, 0, 0);
		ImGui::SameLine();
		ImGui::DragInt(":" DUMMY_LABLE, (int*)&m_endTime.time.minute, scrlspd,  1, 60);
		ImGui::SameLine();
		ImGui::DragInt(DUMMY_LABLE, (int*)&m_endTime.time.second, scrlspd, 1, 60);

		ImGui::Spacing();
		ImGui::Text("Duration:");
		ImGui::InputInt(":" DUMMY_LABLE, (int*)&m_duration.hour, 0, 0); ImGui::SameLine();
		ImGui::DragInt(":" DUMMY_LABLE, (int*)&m_duration.minute, scrlspd, 1, 60); 
		ImGui::SameLine();
		ImGui::DragInt(DUMMY_LABLE, (int*)&m_duration.second, scrlspd, 1, 60);

		ImGui::PopItemWidth();

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Discription:");

		
		static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
		ImGui::InputTextMultiline("##source", &m_description[0], maxDescriptionSize, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 24), flags);

		if (ImGui::Button("Save")) {
			SetVisibility(false);
		}


	}
}
