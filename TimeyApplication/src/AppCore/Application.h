#pragma once
#include "Events/TimeyEvents.h"
#include "BaseWindow.h"
#include "timey_pch.h"

namespace Timey {

	class Application
	{
	public:

		Application();
		~Application();

		inline static Application& getApplication() { return *s_instance; };
		Ref<BaseWindow> getWindowHandle() { return m_window; };

		void operator=(const Application&) = delete;

		void Run();
		void onEvent(Event& e);
		

	private:
		void _Init();
		bool _CloseWindow(WindowCloseEvent& e);

	private:

		static Application* s_instance;
		bool _is_running = true;
		float _last_frame_time = 0;

		Ref<BaseWindow> m_window;


	};

	Application* CreateApp();
}

