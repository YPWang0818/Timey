#include "timey_pch.h"
#include "imgui.h"
#include "UIWindows.h"
#include "Application.h"


namespace Timey {

	UIWindow::UIWindow(const  WindowUISettings& settings)
		:m_WindowID(settings.Title), m_settings(settings)
	{
	}
	void UIWindow::SetVisibility(bool visible, void* data)
	{
		m_visible = visible; 

		if (m_visible) m_settings.onShowCallback(data);
		else m_settings.onHideCallback(data);
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
		m_settings.onShowCallback = TIMEY_BIND_CALLBACK(MinimalViewWindow::onWindowShow);
		m_settings.onHideCallback = TIMEY_BIND_CALLBACK(MinimalViewWindow::onWindowHide);
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
			Ref<SessionViewWindow> sessionWindow = std::static_pointer_cast<SessionViewWindow>(getChildWindowByID(m_sessionWindowID));
			if (sessionWindow->IsVisible()) {
				sessionWindow->SetVisibility(false);
			}
			else {
				sessionWindow->setToNewOnShow();
				sessionWindow->SetVisibility(true);
			}
		}

		if (ImGui::Button("Load session"))
		{
			Ref<SessionViewWindow> sessionWindow = std::static_pointer_cast<SessionViewWindow>(getChildWindowByID(m_sessionWindowID));
			if (sessionWindow->IsVisible()) {
				sessionWindow->SetVisibility(false);
			}
			else {
				sessionWindow->setToCachedOnShow();
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

	StandardViewWindow::StandardViewWindow(const WindowUISettings& settings)
		:UIWindow(settings)
	{
		m_settings.onShowCallback = TIMEY_BIND_CALLBACK(StandardViewWindow::onWindowShow);
		m_settings.onHideCallback = TIMEY_BIND_CALLBACK(StandardViewWindow::onWindowHide);
	};

	void StandardViewWindow::Begin()
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

	void StandardViewWindow::End()
	{
		ImGui::End();
	}


	void StandardViewWindow::onUIRender()
	{
		AppContext& ctx = Application::getAppContext();


		ImGui::Text("This is an standard window.");

		if (ImGui::Button("Swith to minimal window."))
			ctx.currentWindowType = MainWindowType::minmal;


		ImGui::ShowDemoWindow();
		ImGui::ShowMetricsWindow();

	}

	void StandardViewWindow::SetToCurrentWindow()
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

		ImGui::InputInt(":" DUMMY_LABLE, &m_startTime.date.year, 0, 0);
		ImGui::SameLine();
		ImGui::DragInt(":" DUMMY_LABLE, &m_startTime.date.month, scrlspd, 1, 12);
		ImGui::SameLine();
		ImGui::DragInt(DUMMY_LABLE, &m_startTime.date.day, scrlspd, 1, 31);

		ImGui::Text("Time (hh:mm:ss) "); 

		ImGui::InputInt(":" DUMMY_LABLE, &m_startTime.time.hour, 0, 0);
		ImGui::SameLine();
		ImGui::DragInt(":" DUMMY_LABLE, &m_startTime.time.minute, scrlspd, 0, 59);
		ImGui::SameLine();
		ImGui::DragInt(DUMMY_LABLE, &m_startTime.time.second, scrlspd, 0, 59);
		ImGui::SameLine();
		ImGui::Text("-");
		ImGui::InputInt(":" DUMMY_LABLE, &m_endTime.time.hour, 0, 0);
		ImGui::SameLine();
		ImGui::DragInt(":" DUMMY_LABLE, &m_endTime.time.minute, scrlspd,  0, 59);
		ImGui::SameLine();

		ImGui::DragInt(DUMMY_LABLE, &m_endTime.time.second, scrlspd, 0, 59);

		ImGui::Spacing();
		ImGui::Text("Duration:");
		ImGui::InputInt(":" DUMMY_LABLE, &m_duration.hour, 0, 0); 
		ImGui::SameLine();
		ImGui::DragInt(":" DUMMY_LABLE, &m_duration.minute, scrlspd, 0, 59);
		ImGui::SameLine();
		ImGui::DragInt(DUMMY_LABLE, &m_duration.second, scrlspd, 0, 59);

		ImGui::PopItemWidth();

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Discription:");

		
		static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
		ImGui::InputTextMultiline("##source", &m_description[0], maxDescriptionSize, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 24), flags);


		if (ImGui::Button("Save")) {
			SetVisibility(false);
		}

		sanitizeInput();

	}

	void SessionViewWindow::onWindowShow(void* data)
	{
		switch (m_flag) {
		case InternalFlags::brandnew_session: loadNewSession(); break;
		case InternalFlags::cached_session: loadSession(); break;	
		}

	}

	void SessionViewWindow::onWindowHide(void* data)
	{
		pushNewSession();
	}

	void SessionViewWindow::pushNewSession()
	{

		Ref<Session> newSess = CreateRef<Session>();

		newSess->name.resize(maxTitleSize);
		newSess->discription.resize(maxDescriptionSize);

		strcpy(newSess->name.data(), m_title.data());
		strcpy(newSess->discription.data(), m_description.data());
		//newSess->start_time = m_startTime;
		//newSess->end_time = m_endTime;
		newSess->duration = (float)m_duration.toSeconds();
		newSess->ID = 0;


		AppContext& ctx = Application::getAppContext();
		ctx.sessionBuf.savedSessionList.push_back(newSess);
		ctx.sessionBuf.markAsDirty();

	}
	

	// returns 0 if input is valid otherwise return 1. 
	int SessionViewWindow::sanitizeInput()
	{
		bool isVaild = true;

		if (!m_startTime.date.isVaild()) { m_startTime.date.clipToVaild(); isVaild && false; };
		if (!m_startTime.time.isVaild()) { m_startTime.time.clipToVaild(); isVaild && false; };
		if (!m_endTime.date.isVaild()) { m_endTime.date.clipToVaild(); isVaild && false; };
		if (!m_endTime.time.isVaild()) { m_endTime.time.clipToVaild(); isVaild && false; };
		if (!m_duration.isVaild()) { m_duration.clipToVaild(); isVaild && false; };

		return (isVaild == true ? 0 : 1);
	}

	void SessionViewWindow::loadNewSession()
	{
		// The data should be read form the timer;
		m_startTime.setToNow();
		m_endTime.setToNow();
		m_description.clear();
		m_title.clear();

	}


	void SessionViewWindow::loadSession()
	{
		AppContext& ctx = Application::getAppContext();
		RefList<Session> sesList = ctx.sessionBuf.defaultSessionList;

		if (!sesList.size()) {
			loadNewSession();
			return;
		};
		
		strcpy(m_description.data(), sesList[0]->discription.data());
		m_duration = Time::secondsInTime(sesList[0]->duration);
		//m_startTime = sesList[0]->start_time;
		//m_endTime = sesList[0]->end_time;
		strcpy(m_title.data(), sesList[0]->name.data());

		return;

	}
}
