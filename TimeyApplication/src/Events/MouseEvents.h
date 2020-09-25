#pragma once
#include "timey_pch.h"
#include "Event.h"

namespace Timey {

	class  MouseMovedEvent : public Event 
	{
	public:

		_EVENT_TYPE_DECORATION(mouseMoved)
		_EVENT_CATEGORY_DECORATION(mouseEvents)

		MouseMovedEvent(int xPos, int yPos)
			:m_xPos(xPos), m_yPos(yPos)
		{};
		

		inline int getXPos() const { return m_xPos; };
		inline int getYPos() const { return m_yPos; };

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: ( " << m_xPos << " , " << m_yPos <<  " )";

			return ss.str();
		}
	private:
		int m_xPos;
		int m_yPos;
	};

	class  MouseScrolledEvent : public Event {

	public:
		_EVENT_TYPE_DECORATION(mouseScrolled)
		_EVENT_CATEGORY_DECORATION(mouseEvents)

		MouseScrolledEvent(int xOffset, int yOffset)
			:m_xOffset(xOffset), m_yOffset(yOffset)
		{};

		inline int getxOffset() const { return m_xOffset; };
		inline int getyOffset() const { return m_yOffset; };

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: ( " << m_xOffset << " , " << m_yOffset << " )";

			return ss.str();
		};


	private:
		int m_xOffset;
		int m_yOffset;

	};

	class  MouseButtonEvents : public Event
	{
		// Abstract class for mouse button events.
	public:
		_EVENT_CATEGORY_DECORATION(mouseEvents)
		inline int getMosueButton() const { return m_button; };
	
	protected:
		MouseButtonEvents(int button)
			: m_button(button)
		{};

		int m_button;

	};


	class  MousePressedEvent : public MouseButtonEvents
	{
		// Abstract class for mouse button events.
	public:
		_EVENT_TYPE_DECORATION(mousePressed)
		MousePressedEvent(int button)
			: MouseButtonEvents(button)
		{};

		std::string toString() const override {
			std::stringstream ss;
			ss << "MousePressedEvent:" << m_button;

			return ss.str();
		};


	};

	class  MouseReleasedEvent : public MouseButtonEvents
	{
		// Abstract class for mouse button events.
	public:
		_EVENT_TYPE_DECORATION(mouseReleased)
			MouseReleasedEvent(int button)
			: MouseButtonEvents(button)
		{};

		std::string toString() const override {
			std::stringstream ss;
			ss << "MouseReleasedEvent:" << m_button;

			return ss.str();
		};


	};
}
