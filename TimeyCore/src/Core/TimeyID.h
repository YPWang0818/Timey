#pragma once
#include "timey_pch.h"

// A UUID (universally unique identifier) class for various purpose. Currently it will generated a random 64bit number or generated from hashed string. 
// TODO: ID collison detection.


namespace Timey {



	class TimeyID {

	public:
		TimeyID();
		TimeyID(std::size_t ID);
		TimeyID(const std::string& str2hash);
		TimeyID(const TimeyID& other);

		operator std::size_t() { return m_ID; };
		operator const std::size_t() const { return m_ID; }

		bool operator==(const TimeyID& other);

	private:
		std::size_t m_ID;
	};





}

// Costum Hash function for unordered map

namespace std {
	template<>
	struct hash<Timey::TimeyID>
	{
		std::size_t operator()(const std::size_t& uuid) const noexcept
		{
			return std::hash<std::size_t>{}((std::size_t)uuid);
		};
	};
}