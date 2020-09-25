#include "timey_pch.h"
#include "Platform/Win32/Win32Windows.h"


namespace Timey {

	#define TIMEY_USE_GLFW 

	Ref<BaseWindow> BaseWindow::Create(const WindowProps& props, Ref<GraphicsContex> shared_contex) {

		#if defined (TIMEY_USE_GLFW)
		return CreateRef<WindowsWindow>(props, shared_contex);
		#else
		TIMEY_CORE_ASSERT(false, "UGE currently only support GLFW.");
		return nullptr;
		#endif
	};


	Ref<BaseWindow> BaseWindow::Create(const WindowProps& props) {

		#if defined (TIMEY_USE_GLFW)
		return CreateRef<WindowsWindow>(props);
		#else
		TIMEY_CORE_ASSERT(false, "UGE currently only support GLFW.");
		return nullptr;
		#endif
	};






}