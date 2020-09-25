#pragma once
#include "timey_pch.h"
#include "Events/TimeyEvents.h"
#include "Core/Utilty.h"


namespace Timey {


	using EventCallbackFn = std::function<void(Event&)>;


	struct  WindowProps {

		std::string Title;
		unsigned int Width;
		unsigned int Hight;
		std::pair<int, int> Position;

		EventCallbackFn callback_fun;

		bool isVisble = true;
		bool isDecorated = true;
		bool isFocusedOnCreate = true;
		bool isFocusedOnShow = true;
		bool isFloating = false;
		bool isVync = true;

		WindowProps(
			std::string title = "Timey",
			unsigned int width = 1280,
			unsigned int hight = 720,
			std::pair<int, int> pos = { 100, 100 }
		)
			: Title(title), Width(width), Hight(hight), Position(pos), callback_fun(nullptr)
		{}
	};

	class GraphicsContex {

	public:
		~GraphicsContex() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void* getContexWindow() = 0;
		virtual void makeContexCurrent() = 0;
	};


	class  BaseWindow
	{
	public:

		virtual ~BaseWindow() {};

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHight() const = 0;
		virtual const std::pair<int, int>& getPos() const = 0;
		virtual void* getNativeWindow() const = 0;
		virtual const WindowProps& getWindowProps() const = 0;
		virtual Ref<GraphicsContex> getGraphicsContex() const = 0;

		virtual bool isFocused() = 0;
		virtual bool isMinimized() = 0;
		virtual bool isVSync() const = 0;

		virtual void FocusWindow() = 0; // Focus the window, if the window is visible and not iconfied. 

		virtual void setSize(unsigned int width, unsigned hight) = 0;
		virtual void setPos(const std::pair<int, int> pos) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual void setVisbility(bool isVisible) = 0; // Specify if the window is visible to the user or not. 
		virtual void setDecorated(bool isDecorated) = 0; // Specify if the window has decorations such as a border, a close widget, etc. 
		virtual void setFocusedOnShow(bool isFocusedOnShow) = 0; // Specify if the window will be focused initially when it is set to visible. 
		virtual void setTopMost(bool isFloating) = 0; // Specify if the window will always be on top.
		virtual void setTitle(const std::string& title) = 0;
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;


		static Ref<BaseWindow> Create(const WindowProps& props, Ref<GraphicsContex> shared_contex);
		static Ref<BaseWindow> Create(const WindowProps& props = WindowProps());

	};











}