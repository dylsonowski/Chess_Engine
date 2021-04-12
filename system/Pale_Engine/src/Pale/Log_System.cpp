#include "spdlog/sinks/stdout_color_sinks.h"
#include "Log_System.h"

namespace Pale {
	std::shared_ptr<spdlog::logger> Log_System::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log_System::s_clientLogger;

	void Log_System::Init()	{
		spdlog::set_pattern("%^[%l] [%T] %n: %v%$");
		s_coreLogger = spdlog::stdout_color_mt("PALE");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);		
	}
}