#include "timey_pch.h"
#include "Application.h"
#include "TimeyCore.h"

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

			TIMEY_TRACE("Application Running");

		}
	}

	void Application::_Init()
	{
		TIMEY_INFO("Application Initialized.");
	}


	Application* CreateApp() {

		TIMEY_INFO("Application Created!");
		return new Application;
	}
}