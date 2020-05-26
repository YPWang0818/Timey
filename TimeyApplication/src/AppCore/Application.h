#pragma once

namespace Timey {

	class Application
	{
	public:

		Application();
		~Application();

		inline static Application& getApplication() { return *s_instance; };
		void operator=(const Application&) = delete;

		void Run();
	private:
		void _Init();

		static Application* s_instance;
		bool _is_running = true;


	};

	Application* CreateApp();
}

