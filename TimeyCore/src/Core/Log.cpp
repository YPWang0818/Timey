#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Timey {

	std::shared_ptr<spdlog::logger> Log::s_corelogger;
	std::shared_ptr<spdlog::logger> Log::s_clientlogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%l %T] %n: %v%$");

		Log::s_corelogger = spdlog::stdout_color_mt("TIMEY CORE");
		s_corelogger->set_level(spdlog::level::trace);

		Log::s_clientlogger = spdlog::stdout_color_mt("APP");
		s_clientlogger->set_level(spdlog::level::trace);

	}


}