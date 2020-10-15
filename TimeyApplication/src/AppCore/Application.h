#pragma once
#include "Events/TimeyEvents.h"
#include "BaseWindow.h"
#include "UI/UILayer.h"

namespace Timey {

	enum class MainWindowType {
		minmal = 0,
		standard = 1
	};

	struct AppContext {

		bool initialized;
		bool isRunning;

		MainWindowType currentWindowType;
		WindowProps minimalWindowProps, standardWindowProps;



		Ref<BaseWindow> mainWindows;
		Ref<UILayer> UILayer;

		AppContext() {
			initialized = false;
			isRunning = false;
			mainWindows = nullptr;
			UILayer = nullptr;
		
		}


	};



	class Application
	{
	public:

		Application();
		~Application();

		inline static Application& getApplication() { return *s_instance; };
		inline static AppContext& getAppContext() { return *timeyCtx; };

		Ref<BaseWindow> getWindowHandle() { return timeyCtx->mainWindows; };

		void operator=(const Application&) = delete;

		void Run();
		void onEvent(Event& e);
		

	private:
		void Init();
		bool CloseWindow(WindowCloseEvent& e);

	private:

		static Application* s_instance;
		static AppContext* timeyCtx;

		float lastFrameTime = 0;




	};

	Application* CreateApp();


	

}
	

