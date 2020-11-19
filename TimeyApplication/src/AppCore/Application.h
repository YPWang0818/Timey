#pragma once
#include "Events/TimeyEvents.h"
#include "BaseWindow.h"
#include "UI/UILayer.h"

namespace Timey {

	enum class MainWindowType {
		minmal = 0,
		standard = 1
	};

	enum class InfoStatus {
		clean = 0,
		dirty = 1
	};

	struct SessionBuffer
	{
		void markAsDirty() { bufferstatus = InfoStatus::dirty; };
		void markAsClean() { bufferstatus = InfoStatus::clean; };
		bool isDirty() { return (bufferstatus == InfoStatus::dirty) ? true : false; };
		
		RefList<Session> defaultSessionList; 
		RefList<Session> modifiedSessionList;
		RefList<Session> savedSessionList;

		SessionBuffer() {
		
			defaultSessionList.reserve(maxDefaultSessionListSize);
		}

	private:

		static const size_t maxDefaultSessionListSize = 20;
		InfoStatus bufferstatus = InfoStatus::clean;

	};

	struct AppContext {

		bool initialized;
		bool isRunning;

		MainWindowType currentWindowType;
		WindowProps baseWindowSettings;

		WindowUISettings windowUISettings[2];
		WindowUISettings* initWindowUISettings;

		Ref<BaseWindow> mainWindow;
		Ref<UILayer> UILayer;

		SessionBuffer sessionBuf;

		AppContext() {

			initWindowUISettings = nullptr;
			initialized = false;
			isRunning = false;
			mainWindow = nullptr;
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

		Ref<BaseWindow> getWindowHandle() { return timeyCtx->mainWindow; };

		void operator=(const Application&) = delete;

		void Run();
		void onEvent(Event& e);
		

	private:
		void Init();
		bool CloseWindow(WindowCloseEvent& e);
		void UpdateMainWindowType();
		void UptateSessionBuffer();


	private:

		static Application* s_instance;
		static AppContext* timeyCtx;

		float lastFrameTime = 0;




	};

	Application* CreateApp();


	

}
	

