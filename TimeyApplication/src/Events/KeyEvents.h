#pragma once
#include "timey_pch.h"
#include "Event.h"


namespace Timey{

	class  KeyEvents : public Event
	{
	public:
		_EVENT_CATEGORY_DECORATION(keyEvents)

		inline int getKeyCode() const { return m_keyCode; };
	protected:
		int m_keyCode;

		KeyEvents(int keyCode)
			:m_keyCode(keyCode)
		{

		};

	};

	class  KeyPressedEvent :public KeyEvents
	{
	public:
		_EVENT_TYPE_DECORATION(keyPressed)

		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvents(keyCode), m_repeatCount(repeatCount)
		{

		};

		inline int getRepeatCount() const { return m_repeatCount; };
		std::string toString() const override{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

	private:
		int m_repeatCount;

	};

	class  KeyReleasedEvent : public KeyEvents
	{
	public:
		_EVENT_TYPE_DECORATION(keyReleased)

		KeyReleasedEvent(int keyCode)
			:KeyEvents(keyCode)
		{

		};

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode;
			return ss.str();
		};

	};


	class  KeyTypedEvent : public KeyEvents
	{
	public:
		_EVENT_TYPE_DECORATION(keyTyped)

		KeyTypedEvent (int keyCode)
			:KeyEvents(keyCode)
		{

		};

		std::string toString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_keyCode;
			return ss.str();
		};

	};

}
