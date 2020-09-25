#include "timey_pch.h"
#include "Application.h"
#include "TimeyCore.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Timey {

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		TIMEY_CORE_ASSERT(!s_instance, "more than one application is initialized.");

		s_instance = this;
		_Init();
	}

	Application::~Application()
	{


	}

	void Application::Run()
	{
		while (_is_running) {

			//TIMEY_TRACE("Application Running");

			float  this_frame_time = glfwGetTime();
			float ts = this_frame_time - _last_frame_time;
			_last_frame_time = this_frame_time;

			

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			
			m_window->onUpdate();

		}
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.DispatchEvents<WindowCloseEvent>(TIMEY_BIND_CALLBACK(Application::_ShutDown));

		TIMEY_TRACE("{0}", e);
	}


	void Application::_Init()
	{
		TIMEY_INFO("Application Initialized.");

		m_window = BaseWindow::Create();
		m_window->setEventCallback(TIMEY_BIND_CALLBACK(Application::onEvent));


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	bool Application::_ShutDown(WindowCloseEvent& e)
	{
		_is_running = false;

		return true;
	}


	Application* CreateApp() {

		TIMEY_INFO("Application Created!");
		return new Application;
	}
}