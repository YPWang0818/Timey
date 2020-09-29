#include "timey_pch.h"
#include "glad/glad.h"
#include "Win32Windows.h"
#include "GLFW/glfw3.h"
#include "Events/TimeyEvents.h"


namespace Timey {

	static bool _glfw_initialized = false; // A tag to record if GLFW is initialized.
	static void _glfw_custom_error_callback(int error_code, const char* description) {

		TIMEY_CORE_ERROR("GLFW error ({0}) {1}", error_code, description);
	};



	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props, nullptr);
	};

	WindowsWindow::WindowsWindow(const WindowProps& props, Ref<GraphicsContex> shared_contex)
	{
		Init(props, shared_contex);
	};

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::setSize(unsigned int width, unsigned hight)
	{
		glfwSetWindowSize(m_window, width, hight);

	}
	void WindowsWindow::setPos(const std::pair<int, int> pos)
	{
		glfwSetWindowPos(m_window, pos.first, pos.second);

	};




	void WindowsWindow::Init(const WindowProps& props, Ref<GraphicsContex> shared_contex) {

		m_settings = props;

		TIMEY_CORE_INFO("Creating Window: {0} {1} {2}", props.Title, props.Hight, props.Width);

		setVSync(m_settings.isVync);


		glfwWindowHint(GLFW_VISIBLE, m_settings.isVisble);
		glfwWindowHint(GLFW_DECORATED, m_settings.isDecorated);
		glfwWindowHint(GLFW_FOCUSED, m_settings.isFocusedOnCreate);
		glfwWindowHint(GLFW_FLOATING, m_settings.isFloating);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, m_settings.isFocusedOnShow);

		if (!(_glfw_initialized))
		{
			int glfw_success = glfwInit();
			TIMEY_CORE_ASSERT(glfw_success, "Could not initalized glfw!");
			glfwSetErrorCallback(_glfw_custom_error_callback);
			_glfw_initialized = true;

		}

		GLFWwindow* shared_window = nullptr;
		if (shared_contex) shared_window = (GLFWwindow*)shared_contex->getContexWindow();

		m_window = glfwCreateWindow((int)props.Width, (int)props.Hight, props.Title.c_str(), nullptr, shared_window);
		setPos(props.Position);

		m_contex = CreateRef<OpenGLContex>(m_window);
		m_contex->Init();


		glfwSetWindowUserPointer(m_window, &m_settings);


		//set glfw callbacks.

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int hight)
			{
				WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Hight = hight;

				WindowResizeEvent event(width, hight);

				event.WindowHandle = (void*)window;

				data.callback_fun(event);

			});

		glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int xpos, int ypos) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.Position = std::pair<int, int>(xpos, ypos);

			WindowMovedEvent event(xpos, ypos);
			event.WindowHandle = (void*)window;
			data.callback_fun(event);

			});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			event.WindowHandle = (void*)window;
			data.callback_fun(event);

			});

		glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

			if (focused)
			{
				WindowFocusEvent event;
				event.WindowHandle = (void*)window;
				data.callback_fun(event);
			}
			else
			{
				WindowLostFocusEvent event;
				event.WindowHandle = (void*)window;
				data.callback_fun(event);
			};

			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scan_code, int action, int mods)
			{
				WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

				switch (action) {

				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					event.WindowHandle = (void*)window;
					data.callback_fun(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					event.WindowHandle = (void*)window;
					data.callback_fun(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1); //TODO: retrive repeat count.  
					event.WindowHandle = (void*)window;
					data.callback_fun(event);
					break;
				}
				};

			});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int c) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(c);
			event.WindowHandle = (void*)window;
			data.callback_fun(event);
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

			switch (action) {

			case GLFW_PRESS:
			{
				MousePressedEvent event(button);
				event.WindowHandle = (void*)window;
				data.callback_fun(event);
				break;

			}
			case GLFW_RELEASE:
			{
				MouseReleasedEvent event(button);
				event.WindowHandle = (void*)window;
				data.callback_fun(event);
				break;
			}

			};

			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x_pos, double y_pos) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)x_pos, (float)y_pos);
			event.WindowHandle = (void*)window;
			data.callback_fun(event);

			});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			event.WindowHandle = (void*)window;

			data.callback_fun(event);

			});

	};

	void WindowsWindow::ShutDown()
	{

		glfwDestroyWindow(m_window);
	};


	void WindowsWindow::onUpdate()
	{
		m_contex->makeContexCurrent();
		glfwPollEvents();

		m_contex->SwapBuffers();

	};

	void WindowsWindow::setVSync(bool enabled) {

		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_settings.isVync = enabled;
	};

	bool WindowsWindow::isVSync() const {
		return m_settings.isVync;
	}

	void WindowsWindow::setVisbility(bool isVisible)
	{
		isVisible ? glfwShowWindow(m_window) : glfwHideWindow(m_window);
		m_settings.isVisble = isVisible;
	};

	void WindowsWindow::setDecorated(bool isDecorated)
	{
		isDecorated ? glfwSetWindowAttrib(m_window, GLFW_DECORATED, GL_TRUE) : glfwSetWindowAttrib(m_window, GLFW_DECORATED, GL_FALSE);
		m_settings.isDecorated = isDecorated;
	}

	bool WindowsWindow::isFocused()
	{
		return (bool)glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
	}

	bool WindowsWindow::isMinimized()
	{
		return (bool)glfwGetWindowAttrib(m_window, GLFW_ICONIFIED);
	}

	void WindowsWindow::FocusWindow()
	{
		glfwFocusWindow(m_window);
	}

	void WindowsWindow::setFocusedOnShow(bool isFocusedOnShow)
	{
		isFocusedOnShow ? glfwSetWindowAttrib(m_window, GLFW_FOCUS_ON_SHOW, GL_TRUE) : glfwSetWindowAttrib(m_window, GLFW_FOCUS_ON_SHOW, GL_FALSE);
		m_settings.isFocusedOnShow = isFocusedOnShow;
	}

	void WindowsWindow::setTopMost(bool isFloating)
	{
		isFloating ? glfwSetWindowAttrib(m_window, GLFW_FLOATING, GL_TRUE) : glfwSetWindowAttrib(m_window, GLFW_FLOATING, GL_FALSE);
		m_settings.isFloating = isFloating;
	}

	void WindowsWindow::setTitle(const std::string& title)
	{
		m_settings.Title = title;
		glfwSetWindowTitle(m_window, title.c_str());
	}






	/////////////////////////// Contex /////////////////////////////


	OpenGLContex::OpenGLContex(GLFWwindow* window_handle)
		: m_window(window_handle)
	{

	}

	void OpenGLContex::Init()
	{
		glfwMakeContextCurrent(m_window);
		int glad_success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		TIMEY_CORE_ASSERT(glad_success, "Failed to initialize OpenGL context.");

		TIMEY_CORE_INFO("OpenGL Render: {0}", glGetString(GL_RENDERER));
		TIMEY_CORE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));
	}

	void* OpenGLContex::getContexWindow() {
		return (void*)m_window;
	}

	void OpenGLContex::makeContexCurrent()
	{
		glfwMakeContextCurrent(m_window);
	}

	void OpenGLContex::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}














}