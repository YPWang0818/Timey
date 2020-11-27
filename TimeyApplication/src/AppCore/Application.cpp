#include "timey_pch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "TimeyCore.h"
#include  "UI/UILayer.h"
#include "Application.h"


namespace Timey {

	Application* Application::s_instance = nullptr;
	AppContext* Application::timeyCtx = new AppContext;

	Application::Application()
	{
		TIMEY_CORE_ASSERT(!s_instance, "more than one application is initialized.");
		s_instance = this;

		Init();
	}

	Application::~Application()
	{
		timeyCtx->UILayer->OnDistory();

	}

	void Application::Run()
	{
		
		while (timeyCtx->isRunning) {

			UpdateMainWindowType();
			UptateSessionBuffer();

			float  thisFrameTime = glfwGetTime();
			float ts = thisFrameTime - lastFrameTime;
			lastFrameTime = thisFrameTime;
			
			timeyCtx->mainWindow->onUpdate();
			timeyCtx->UILayer->OnUpdate(ts);
			

		}
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.DispatchEvents<WindowCloseEvent>(TIMEY_BIND_CALLBACK(Application::CloseWindow));
		timeyCtx->UILayer->OnEvent(e);
		
	}


	void Application::Init()
	{

		WindowUISettings minwinSetting = { 500, 200, "Minimal Window"};

		WindowUISettings stdwinSetting = { 1280, 780, "Timey"};

		timeyCtx->windowUISettings[0] = minwinSetting;
		timeyCtx->windowUISettings[1] = stdwinSetting;
		timeyCtx->currentWindowType = MainWindowType::minmal;

		timeyCtx->initWindowUISettings = &timeyCtx->windowUISettings[(uint32_t)timeyCtx->currentWindowType];
		
		WindowProps basewinSetting;
		basewinSetting.callback_fun = TIMEY_BIND_CALLBACK(Application::onEvent);
		basewinSetting.Title = minwinSetting.Title;
		basewinSetting.Width = minwinSetting.Width;
		basewinSetting.Hight = minwinSetting.Hight;

		timeyCtx->baseWindowSettings = basewinSetting;

		if (!timeyCtx->initialized) {
		
			timeyCtx->mainWindow = BaseWindow::Create(timeyCtx->baseWindowSettings);
			timeyCtx->UILayer = CreateRef<UILayer>();
			timeyCtx->UILayer->OnInit();
			timeyCtx->mainWindow->getGraphicsContex()->makeContexCurrent();
			timeyCtx->isRunning = true;
			timeyCtx->initialized = true;

		}

	
	}

	bool Application::CloseWindow(WindowCloseEvent& e)
	{
		timeyCtx->isRunning = false;
		return true;
	}

	void Application::UpdateMainWindowType()
	{
		Ref<UIWindow> minimalWindow = timeyCtx->UILayer->getWindowByName(timeyCtx->windowUISettings[(uint32_t)MainWindowType::minmal].Title);
		Ref<UIWindow> standardWindow = timeyCtx->UILayer->getWindowByName(timeyCtx->windowUISettings[(uint32_t)MainWindowType::standard].Title);

	
		for (auto window : *(timeyCtx->UILayer)) {
			window.second->SetVisibility(false);
		
		}; // Set all windows to not visible at first.

		switch (timeyCtx->currentWindowType)
		{
		case MainWindowType::minmal: minimalWindow->SetToCurrentWindow();
			break;
		case MainWindowType::standard: standardWindow->SetToCurrentWindow();
			break;
		default:
			break;
		};
	}

	void Application::UptateSessionBuffer()
	{
		AppContext& ctx = Application::getAppContext();


		if (ctx.sessionBuf.isDirty()) {

			auto& defSes = ctx.sessionBuf.defaultSessionList;
			auto& savSes = ctx.sessionBuf.savedSessionList;
			auto& modSes = ctx.sessionBuf.modifiedSessionList;

			for (auto& s : savSes)
			{
				defSes.push_back(s);
			}

			for (auto& s : modSes)
			{
				defSes.push_back(s);
			}

			savSes.clear();
			modSes.clear();
			ctx.sessionBuf.markAsClean();
		};
	}


	Application* CreateApp() {

		TIMEY_INFO("Application Created!");
		return new Application;
	}
}