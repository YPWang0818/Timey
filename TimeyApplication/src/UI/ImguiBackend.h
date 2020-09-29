#pragma once
#include "imgui.h"
#include "Events/TimeyEvents.h"


namespace Timey {


	namespace ImguiBackend {

		void Init();
		void NewFrame();
		void DrawRenderData(ImDrawData* drawdata);
		void ShutDown();


		void OnEvent(Event& e);

		bool KeyPressedCallBack(KeyPressedEvent& e);
		bool KeyReleasedCallBack(KeyReleasedEvent& e);
		bool MousePressedCallBack(MousePressedEvent& e);
		bool MouseReleasedCallBack(MouseReleasedEvent& e);
		bool MouseMovedCallBack(MouseMovedEvent& e);
		bool MouseScrolledCallBack(MouseScrolledEvent& e);
		bool KeyTypedCallBack(KeyTypedEvent& e);
		bool WindowResizeCallBack(WindowResizeEvent& e);
		bool WindowMovedCallBack(WindowMovedEvent& e);




	}

}