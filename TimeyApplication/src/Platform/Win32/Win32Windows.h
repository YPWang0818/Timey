#pragma once
#include "AppCore/BaseWindow.h"
#include "GLFW/glfw3.h"


namespace Timey {


	class OpenGLContex : public GraphicsContex
	{
	public:
		OpenGLContex(GLFWwindow* window_handle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void* getContexWindow() override;
		virtual void makeContexCurrent() override;

	private:
		GLFWwindow* m_window;
	};


	class  WindowsWindow : public BaseWindow
	{
	public:
		WindowsWindow(const WindowProps& props);
		WindowsWindow(const WindowProps& props, Ref<GraphicsContex> shared_contex);

		virtual ~WindowsWindow();


		virtual void onUpdate() override;

		virtual unsigned int getWidth() const override { return m_settings.Width; };
		virtual unsigned int getHight() const override { return m_settings.Hight; };
		virtual const std::pair<int, int>& getPos() const override { return m_settings.Position; };
		virtual void* getNativeWindow() const override { return m_window; };
		virtual const WindowProps& getWindowProps() const override { return m_settings; };
		virtual Ref<GraphicsContex> getGraphicsContex() const override { return m_contex; };

		virtual bool isFocused() override;
		virtual bool isMinimized() override;
		virtual bool isVSync() const override;

		virtual void FocusWindow() override; 

		virtual void setSize(unsigned int width, unsigned hight) override;
		virtual void setPos(const std::pair<int, int> pos) override;
		virtual void setVSync(bool enabled) override;
		virtual void setVisbility(bool isVisible) override; 
		virtual void setDecorated(bool isDecorated) override; 
		virtual void setFocusedOnShow(bool isFocusedOnShow) override; 
		virtual void setTopMost(bool isFloating) override; 
		virtual void setTitle(const std::string& title) override;
		virtual void setEventCallback(const EventCallbackFn& callback) override { m_settings.callback_fun = callback; };



	private:
		virtual void Init(const WindowProps& props, Ref<GraphicsContex> shared_contex);
		virtual void ShutDown();


	private:

		WindowProps m_settings;
		GLFWwindow* m_window;

		Ref<GraphicsContex> m_contex;

	};


}
