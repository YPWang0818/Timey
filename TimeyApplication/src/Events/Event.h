#pragma once
#include "timey_pch.h"
#include "Core/Log.h"

namespace Timey {

	enum class EventType {
		none = 0,

		windowClose, 
		windowResize, 
		windowFocus, 
		windowLostFocus, 
		windowMoved, // window events

		appTick, 
		appUpdate, 
		appRender, //app events,

		keyPressed, 
		keyReleased,
		keyTyped,//key events

		mousePressed, 
		mouseReleased, 
		mouseMoved,
		mouseScrolled //mouseEvents
	};


	# define BIT(x) 1 << x

	enum EventCategory {
		none =			0,
		windowEvents =	BIT(1),
		appEvents =		BIT(2),
		keyEvents =		BIT(3),
		mouseEvents =	BIT(4),
	};




	class  Event {
		// base class for all events
	friend class EventDispatcher;
	public:

		virtual EventType getEventType() const = 0;
		virtual EventCategory getEventCategory() const = 0;
		virtual const char* getName() const = 0;

		// Overriding the these base method using macros.

		#define _EVENT_TYPE_DECORATION(type) static EventType getStaticType(){ return EventType::##type; };\
		virtual EventType getEventType() const override { return getStaticType(); }; \
		virtual const char* getName() const override { return #type; };

		#define _EVENT_CATEGORY_DECORATION(cat) virtual EventCategory getEventCategory() const override {return EventCategory::##cat; };


		virtual inline std::string toString() const { return getName(); };
		inline bool isInCategory(const EventCategory& evtcat) const {
			return (getEventCategory() & evtcat);
		}
		inline bool Handled() const {
			return m_handled;
		}

	public:

		void* WindowHandle = nullptr; 

	protected:
		bool m_handled = false;

	};


	class   EventDispatcher {

	template <typename T>
	using Eventfn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& e)
			: m_event(e)
		{};

		template<typename T>
		bool DispatchEvents(Eventfn<T> fun){
			if (m_event.getEventType() == T::getStaticType()) {
				m_event.m_handled = fun( *(T*)&m_event );

				return true;
			}
			else
			{
				return false;
			}
		
		};

	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.toString();
	};

}

