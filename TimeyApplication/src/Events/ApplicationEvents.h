#pragma once
#include "timey_pch.h"
#include "Event.h"



namespace Timey {

	class  AppTickEvent : public Event
	{
	public:
		_EVENT_TYPE_DECORATION(appTick)
		_EVENT_CATEGORY_DECORATION(appEvents)
	};

	class  AppUpdateEvent : public Event
	{
	public:
		_EVENT_TYPE_DECORATION(appUpdate)
		_EVENT_CATEGORY_DECORATION(appEvents)
	};


	class  AppRenderEvent : public Event
	{
	public:
		_EVENT_TYPE_DECORATION(appRender)
		_EVENT_CATEGORY_DECORATION(appEvents)
	};


}