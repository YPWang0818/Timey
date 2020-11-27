#include "timey_pch.h"
#include "TimeyTime.h"
#include <ctime>

namespace Timey {


	bool Date::isVaild()
	{
		bool test = true;
		test = test && (year > 0) && (day > 0);
		test = test && (month > 0) && (month <= 12);
		test = test && (day <= maxDayByMonth());
		
		return test;
		
	}

	uint32_t Date::maxDayByMonth()
	{
		if (((month < 8) && (month % 2 == 1)) || ((month >= 8) && (month % 2 == 0))) {
			return 31;
		}
		else if (month != 2) {
			return 30;
		}
		else {
			if (isLeapYear()) {
				return 29;
			}
			else {
				return 28;
			}
		}

		return 0;
	}

	bool Date::isLeapYear()
	{ 
		if (year % 400 == 0)
			return true;
		if (year % 100 == 0)
			return false;
		if (year % 4 == 0)
			return true;

		return false;

	}

	void Date::clipToVaild()
	{

		year = (year < 0) ? 0 : year;
		month = (month < 0) ? 0 : month;
		day = (day < 0) ? 0 : day;

		month = (month > 12) ? 12 : month;
		uint32_t maxDay = maxDayByMonth();
		day = (day > maxDay) ? maxDay : day;
	}

	void Date::setToNow()
	{
		std::time_t rawtime = std::time(nullptr);
		std::tm* now = std::localtime(&rawtime);

		year = now->tm_year + 1900;
		month = now->tm_mon + 1;
		day = now->tm_mday;

	}

	bool Time::isVaild()
	{
		return (hour > 0) && isMinSecValid();
	}

	void Time::clipToVaild()
	{
		minute = (minute < 0) ? 0 : minute;
		minute = (minute > 59) ? 59 : minute;
		second = (second < 0) ? 0 : second;
		second = (second > 59) ? 59 : second;

	}

	int32_t Time::toSeconds()
	{
		return (3600 * hour + 60 * minute + second);
	}

	Time Time::secondsInTime(int32_t seconds)
	{
		TIMEY_CORE_ASSERT(seconds >= 0, "secondsInTime currently dosen't support negative time");

		int32_t hr = seconds / 3600;
		int32_t min = ( seconds  - hr * 3600) / 60;
		int32_t sec = (seconds - hr * 3600 - min * 60);

		return { Hour(hr), Minute(min), Second(sec) };
	}

	bool Time::isMinSecValid()
	{
		return (minute >= 0) && (second >= 0) && (minute < 60) && (second < 60);
	}

	bool ClockTime::isVaild()
	{
		return (hour >= 0) && (hour < 24) && isMinSecValid();
	}

	void ClockTime::clipToVaild()
	{
		minute = (minute < 0) ? 0 : minute;
		minute = (minute > 59) ? 59 : minute;
		second = (second < 0) ? 0 : second;
		second = (second > 59) ? 59 : second;
		hour = (hour < 0) ? 0 : hour;
		hour = (hour > 23) ? 23 : hour;

	}

	void ClockTime::setToNow()
	{
		// Currently the values is stored as local time. Should stored as UTC in future. 
		std::time_t rawtime = std::time(nullptr);
		std::tm* now = std::localtime(&rawtime);

		hour = now->tm_hour;
		minute = now->tm_min;
		second = now->tm_sec;

	}

}