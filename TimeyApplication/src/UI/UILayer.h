#pragma once
#include "Events/Event.h"

namespace Timey {

	class UILayer {

	public:

		static void OnInit();
		static void OnDistory();
		static void OnEvent(Event& e);
		static void OnUpdate(float timestep);


	};


}