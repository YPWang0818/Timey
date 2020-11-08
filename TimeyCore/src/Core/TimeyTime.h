#pragma once
#include "timey_pch.h"

namespace Timey {

	class Date {
	public:

		Date() { setToNow(); };
		Date(uint32_t Year, uint32_t Month, uint32_t Day)
			:year(Year), month(Month), day(Day)
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

		uint32_t year;
		uint32_t month;
		uint32_t day;

	
	};

	/*
	enum class TimeFlag
	{
		Flag_None = 0,
		Flag_24Hours = 1 << 1
	
	};
	*/


	class Time {

	public:

		Time() = default;
		Time(uint32_t Hour, uint32_t Minute, uint32_t Second)
			:hour(Hour), minute(Minute), second(Second) 
		{
			if(!isVaild()) clipToVaild();
		};


		virtual bool isVaild();
		virtual void clipToVaild();
		

		std::string toString() const {
			std::stringstream ss;
			ss << hour << ":" << minute << ":" << second;
			return ss.str();
		}

	protected:
		bool isMinSecValid();

	public:
		uint32_t hour = 0;
		uint32_t minute = 0;
		uint32_t second = 0;
	};


	class ClockTime : public Time 
	{
		// The ClockTime only support 24 hour format in UTC. 
	public:
		ClockTime() { setToNow(); };
		ClockTime(uint32_t Hour, uint32_t Minute, uint32_t Second)
			:Time(Hour, Minute, Second) {};

		virtual bool isVaild();
		virtual void clipToVaild();
		void setToNow();
	};


	struct DateTime {

		DateTime() = default;
		DateTime(uint32_t Year, uint32_t Month, uint32_t Day, uint32_t Hour, uint32_t Minute, uint32_t Second)
			:date{ Year , Month,  Day }, time{ Hour , Minute, Second } {};


		Date date;
		Time time;

		std::string toString() const {
			std::stringstream ss;
			ss << date.toString() << " - " << time.toString();
			return ss.str();
		}
	};






}