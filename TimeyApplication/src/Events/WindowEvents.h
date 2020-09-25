#pragma once
#include "timey_pch.h"
#include "Event.h"

namespace Timey {


	class  WindowCloseEvent : public Event
	{
	public:
		_EVENT_TYPE_DECORATION(windowClose)
		_EVENT_CATEGORY_DECORATION(windowEvents)

	};


	class  WindowResizeEvent : public Event
	{
	public:

		_EVENT_TYPE_DECORATION(windowResize)
		_EVENT_CATEGORY_DECORATION(windowEvents)

		WindowResizeEvent(unsigned int width, unsigned int hight)
			:m_width(width), m_hight(hight)
		{

		};
		
		inline unsigned int getWidth() const{ return m_width; };
		inline unsigned int getHight() const{ return m_hight; };

		std::string toString() const override {
			std::stringstream ss;
			ss << "WindowResize Event: Width " << m_width << " Hight " << m_hight;

			return ss.str();
		}


	private:
		unsigned int m_width;
		unsigned int m_hight;
	};


	class  WindowFocusEvent : public Event
	{
	public:
		_EVENT_TYPE_DECORATION(windowFocus)
		_EVENT_CATEGORY_DECORATION(windowEvents)

	};



	class  WindowLostFocusEvent : public Event
	{
	public:
		_EVENT_TYPE_DECORATION(windowLostFocus)
		_EVENT_CATEGORY_DECORATION(windowEvents)


	};

	class  WindowMovedEvent : public Event
	{
	public:

		WindowMovedEvent(int xpos, int ypos)
			:m_xpos(xpos), m_ypos(ypos)
		{}

		_EVENT_TYPE_DECORATION(windowMoved)
		_EVENT_CATEGORY_DECORATION(windowEvents)

		inline unsigned int getxPos() const { return m_xpos; };
		inline unsigned int getyPos() const { return m_ypos; };

		std::string toString() const override {
			std::stringstream ss;
			ss << "WindowMoved Event: Position ( " << m_xpos << " , " << m_ypos << ")";

			return ss.str();
		}
	
	private:
		int m_xpos, m_ypos;

	};
}