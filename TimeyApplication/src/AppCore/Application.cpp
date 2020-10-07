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

			float  thisFrameTime = glfwGetTime();
			float ts = thisFrameTime - lastFrameTime;
			lastFrameTime = thisFrameTime;

			
			timeyCtx->mainWindows[(uint32_t)timeyCtx->currentWindowType]->onUpdate();
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


		timeyCtx->minimalWindowProps.Title = "Minimal Window";
		timeyCtx->minimalWindowProps.Hight = 200;
		timeyCtx->minimalWindowProps.Width = 500;
		timeyCtx->minimalWindowProps.callback_fun = TIMEY_BIND_CALLBACK(Application::onEvent);

		timeyCtx->standardWindowProps.Title = "Timey";
		timeyCtx->standardWindowProps.Hight = 780;
		timeyCtx->standardWindowProps.Width = 1280;
		timeyCtx->standardWindowProps.callback_fun = TIMEY_BIND_CALLBACK(Application::onEvent);



		if (!timeyCtx->initialized) {
		
			timeyCtx->currentWindowType = MainWindowType::standard;
			timeyCtx->mainWindows[(uint32_t)MainWindowType::standard] = BaseWindow::Create(timeyCtx->standardWindowProps);
			timeyCtx->mainWindows[(uint32_t)MainWindowType::standard]->setVisbility(false);

			timeyCtx->mainWindows[(uint32_t)MainWindowType::minmal] = BaseWindow::Create(timeyCtx->minimalWindowProps);
			timeyCtx->mainWindows[(uint32_t)MainWindowType::minmal]->setVisbility(false);

			timeyCtx->UILayer = CreateRef<UILayer>();
			timeyCtx->UILayer->OnInit();


			timeyCtx->mainWindows[(uint32_t)timeyCtx->currentWindowType]->getGraphicsContex()->makeContexCurrent();

			timeyCtx->mainWindows[(uint32_t)timeyCtx->currentWindowType]->setVisbility(true);
			
			timeyCtx->isRunning = true;
			timeyCtx->initialized = true;

		}

	
	}

	bool Application::CloseWindow(WindowCloseEvent& e)
	{
		timeyCtx->isRunning = false;
		return true;
	}


	Application* CreateApp() {

		TIMEY_INFO("Application Created!");
		return new Application;
	}
}