#pragma once
#include "Events/Event.h"
#include "Windows.h"

namespace Timey {

	class UILayer {

	public:
	
		void OnInit();
		void OnDistory();
		void OnEvent(Event& e);
		void OnUpdate(float timestep);

		void PushWindow(const Ref<Window>& window);


	private:
		std::vector<Ref<Window>> m_window_list;


	};

}