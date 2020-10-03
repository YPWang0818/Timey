#pragma once
#include "Events/Event.h"
#include "Windows.h"

namespace Timey {

	class UILayer {

	public:

		static void OnInit();
		static void OnDistory();
		static void OnEvent(Event& e);
		static void OnUpdate(float timestep);

		static void PushWindow(const Ref<Window>& window);


	private:
		static std::vector<Ref<Window>> m_window_list;


	};

}