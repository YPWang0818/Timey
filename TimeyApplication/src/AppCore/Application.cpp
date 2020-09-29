#include "timey_pch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "TimeyCore.h"
#include  "UI/UILayer.h"
#include "Application.h"


namespace Timey {

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		TIMEY_CORE_ASSERT(!s_instance, "more than one application is initialized.");

		s_instance = this;

		_Init();
		UILayer::OnInit();
	}

	Application::~Application()
	{
		UILayer::OnDistory();

	}

	void Application::Run()
	{
		while (_is_running) {

			float  this_frame_time = glfwGetTime();
			float ts = this_frame_time - _last_frame_time;
			_last_frame_time = this_frame_time;

			

		
			
			UILayer::OnUpdate(ts);
			m_window->onUpdate();

		}
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.DispatchEvents<WindowCloseEvent>(TIMEY_BIND_CALLBACK(Application::_CloseWindow));

		UILayer::OnEvent(e);
		//TIMEY_TRACE("{0}", e);
	}


	void Application::_Init()
	{
		TIMEY_INFO("Application Initialized.");

		m_window = BaseWindow::Create();
		m_window->setEventCallback(TIMEY_BIND_CALLBACK(Application::onEvent));

	}

	bool Application::_CloseWindow(WindowCloseEvent& e)
	{
		_is_running = false;

		return true;
	}


	Application* CreateApp() {

		TIMEY_INFO("Application Created!");
		return new Application;
	}
}