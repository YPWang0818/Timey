#pragma once
#include "timey_pch.h"
#include <chrono>

namespace Timey {

	template<typename T>
	class GenericTimeVal {
	public:

		GenericTimeVal(int val)
			:time_val{static_cast<T>(val)}
		{};

		GenericTimeVal() = default;

		operator int() {
			return (int)time_val;
		};
		operator const int() const { 
			return (const int)time_val;
		};

		T* operator&() { return &time_val; };


	private:
		T time_val;
	};

	using Year = GenericTimeVal<int32_t>;
	using Month = GenericTimeVal<int32_t>;
	using Day = GenericTimeVal<int32_t>;
	using Hour = GenericTimeVal<int32_t>;
	using Minute = GenericTimeVal<int32_t>;
	using Second = GenericTimeVal<int32_t>;

	class Date {
	public:

		Date() { setToNow(); };
		Date(Year y, Month m, Day d)
			:year(y), month(m), day(d)
		{
			if (!isVaild()) clipToVaild();
		};

		bool isVaild();
		void clipToVaild();
		void setToNow();

		std::string toString() const {
			std::stringstream ss;
			ss << year << ":" << month << ":" << day;
			return ss.str();
		}

	private:
		uint32_t maxDayByMonth();
		bool isLeapYear();

	public:

		Year year;
		Month month;
		Day day;

	
	};



	class Time {

	public:

		Time() = default;
		Time(Hour hr, Minute min, Second sec)
			:hour(hr), minute(min), second(sec) 
		{
			if(!isVaild()) clipToVaild();
		};


		virtual bool isVaild();
		virtual void clipToVaild();
		virtual int32_t toSeconds();


		static Time secondsInTime(int32_t seconds);

		std::string toString() const {
			std::stringstream ss;
			ss << hour << ":" << minute << ":" << second;
			return ss.str();
		}

	protected:
		bool isMinSecValid();

	public:
		Hour hour = 0;
		Minute minute = 0;
		Second second = 0;
	};


	class ClockTime : public Time 
	{
		// The ClockTime only support 24 hour format. 
	public:
		ClockTime() { setToNow(); };
		ClockTime(Hour hr, Minute min, Second sec)
			:Time(hr, min, sec) {};

		virtual bool isVaild();
		virtual void clipToVaild();
		void setToNow();
	};


	struct DateTime {

		DateTime() = default;
		DateTime(Year y, Month m, Day d, Hour hr, Minute min, Second sec)
			:date{ y , m,  d }, time{ hr , min, sec } {};

		Date date;
		ClockTime time;

		void setToNow() { date.setToNow(); time.setToNow(); };

		std::string toString() const {
			std::stringstream ss;
			ss << date.toString() << " - " << time.toString();
			return ss.str();
		}
	};






}