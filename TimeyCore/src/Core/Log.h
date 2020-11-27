#pragma once
#include"spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <memory>

namespace Timey
{

	class  Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger()
		{
			return s_corelogger;
		};
		inline static std::shared_ptr<spdlog::logger>& getClientLogger()
		{
			return s_clientlogger;
		};
	private:
		static std::shared_ptr<spdlog::logger> s_corelogger;
		static std::shared_ptr<spdlog::logger> s_clientlogger;
	};

};


#define TIMEY_CORE_CRITICAL(...) ::Timey::Log::getCoreLogger()->critical(__VA_ARGS__)
#define TIMEY_CORE_ERROR(...) ::Timey::Log::getCoreLogger()->error(__VA_ARGS__)
#define TIMEY_CORE_WARN(...) ::Timey::Log::getCoreLogger()->warn(__VA_ARGS__)
#define TIMEY_CORE_INFO(...) ::Timey::Log::getCoreLogger()->info(__VA_ARGS__)
#define TIMEY_CORE_TRACE(...) ::Timey::Log::getCoreLogger()->trace(__VA_ARGS__)

#define TIMEY_CRITICAL(...) ::Timey::Log::getClientLogger()->critical(__VA_ARGS__)
#define TIMEY_ERROR(...) ::Timey::Log::getClientLogger()->error(__VA_ARGS__)
#define TIMEY_WARN(...) ::Timey::Log::getClientLogger()->warn(__VA_ARGS__)
#define TIMEY_INFO(...) ::Timey::Log::getClientLogger()->info(__VA_ARGS__)
#define TIMEY_TRACE(...) ::Timey::Log::getClientLogger()->trace(__VA_ARGS__)

#ifdef TIMEY_ENABLE_ASSERT
#define TIMEY_CORE_ASSERT(X, ...) {if(!(X)){ TIMEY_CORE_ERROR("Assertion Failed {0}", __VA_ARGS__);   __debugbreak();} }
#define TIMEY_ASSERT(X, ...) {if(!X){ TIMEY_ERROR("Assertion Failed {0}", __VA_ARGS__);__debugbreak(); } }
#else
#define TIMEY_CORE_ASSERT(X, ...)
#define TIMEY_ASSERT(X, ...)
#endif