#pragma once
#include "Events/Event.h"
#include "UIWindows.h"

namespace Timey {

	using UIWindowMapping = std::unordered_map<TimeyID, Ref<UIWindow>>;

	class UILayer {

	public:
	
		void OnInit();
		void OnDistory();
		void OnEvent(Event& e);
		void OnUpdate(float timestep);

		void PushWindow(const Ref<UIWindow>& window);
		Ref<UIWindow> getWindowByID(const TimeyID& ID);

		auto begin() { return m_WindowLists.begin(); };
		auto end() { return m_WindowLists.end(); };

	private:
		UIWindowMapping m_WindowLists;


	};

}