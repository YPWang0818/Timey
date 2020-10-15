#include "timey_pch.h"
#include "TimeyID.h"


namespace Timey {



	static std::random_device randomDev;
	static std::mt19937 Gen(randomDev());
	static std::uniform_int_distribution<std::size_t> uniformDistribution;

	TimeyID::TimeyID()
		:m_ID(uniformDistribution(Gen))
	{};

	TimeyID::TimeyID(std::size_t ID)
		:m_ID(ID)
	{}

	TimeyID::TimeyID(const TimeyID& other)
		: m_ID(other.m_ID)
	{};

	TimeyID::TimeyID(const std::string& str2hash)
	{
		m_ID = std::hash<std::string>{}(str2hash);
	};


	bool TimeyID::operator==(const TimeyID& other)
	{
		return (this->m_ID == other.m_ID);
	};

}