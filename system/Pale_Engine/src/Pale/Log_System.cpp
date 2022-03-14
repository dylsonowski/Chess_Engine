#include "palepch.h"

namespace Pale {
	std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> Log_System::s_consoleSink;
	std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Log_System::s_fileSink;
	std::shared_ptr<spdlog::logger> Log_System::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log_System::s_clientLogger;
	std::shared_ptr<spdlog::logger> Log_System::s_testsLogger;
	std::shared_ptr<spdlog::logger> Log_System::s_assertionLogger;

	void Log_System::Init(LOGGING_METHOD methodOfLogging) {
		static std::vector<spdlog::sink_ptr> sinks;
		spdlog::set_pattern("%^[%l] [%T] %n: %v%$");

		switch (methodOfLogging) {
		case LOGGING_METHOD::FILE_ONLY: {
			s_fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/ApplicationRuntimeLogs.log", true);
			s_fileSink->set_level(spdlog::level::trace);

			s_coreLogger = std::make_shared<spdlog::logger>("PALE", s_fileSink);
			s_coreLogger->set_level(spdlog::level::trace);

			s_clientLogger = std::make_shared<spdlog::logger>("APP", s_fileSink);
			s_clientLogger->set_level(spdlog::level::trace);

			s_testsLogger = std::make_shared<spdlog::logger>("TEST", s_fileSink);
			s_testsLogger->set_level(spdlog::level::trace);

			s_assertionLogger = std::make_shared<spdlog::logger>("ASSERT", s_fileSink);
			s_assertionLogger->set_level(spdlog::level::trace);
			break;
		}
		case LOGGING_METHOD::TERMINAL_ONLY: {
			s_consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			s_consoleSink->set_level(spdlog::level::trace);

			s_coreLogger = std::make_shared<spdlog::logger>("PALE", s_consoleSink);
			s_coreLogger->set_level(spdlog::level::trace);

			s_clientLogger = std::make_shared<spdlog::logger>("APP", s_consoleSink);
			s_clientLogger->set_level(spdlog::level::trace);

			s_testsLogger = std::make_shared<spdlog::logger>("TEST", s_consoleSink);
			s_testsLogger->set_level(spdlog::level::trace);

			s_assertionLogger = std::make_shared<spdlog::logger>("ASSERT", s_consoleSink);
			s_assertionLogger->set_level(spdlog::level::trace);
			break;
		}
		case LOGGING_METHOD::FILE_AND_TERMINAL: {
			s_consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			s_consoleSink->set_level(spdlog::level::trace);
			sinks.push_back(s_consoleSink);

			s_fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/ApplicationRuntimeLogs.log", true);
			s_fileSink->set_level(spdlog::level::trace);
			sinks.push_back(s_fileSink);

			s_coreLogger = std::make_shared<spdlog::logger>("PALE", sinks.begin(), sinks.end());
			s_coreLogger->set_level(spdlog::level::trace);

			s_clientLogger = std::make_shared<spdlog::logger>("APP", sinks.begin(), sinks.end());
			s_clientLogger->set_level(spdlog::level::trace);

			s_testsLogger = std::make_shared<spdlog::logger>("TEST", sinks.begin(), sinks.end());
			s_testsLogger->set_level(spdlog::level::trace);

			s_assertionLogger = std::make_shared<spdlog::logger>("ASSERT", sinks.begin(), sinks.end());
			s_assertionLogger->set_level(spdlog::level::trace);
			break;
		}
		default: {
			s_consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			s_consoleSink->set_level(spdlog::level::err);
			sinks.push_back(s_consoleSink);

			s_fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/ApplicationRuntimeLogs.log", true);
			s_fileSink->set_level(spdlog::level::trace);
			sinks.push_back(s_fileSink);

			s_coreLogger = std::make_shared<spdlog::logger>("PALE", sinks.begin(), sinks.end());
			s_coreLogger->set_level(spdlog::level::trace);

			s_clientLogger = std::make_shared<spdlog::logger>("APP", sinks.begin(), sinks.end());
			s_clientLogger->set_level(spdlog::level::trace);

			s_testsLogger = std::make_shared<spdlog::logger>("TEST", sinks.begin(), sinks.end());
			s_testsLogger->set_level(spdlog::level::trace);

			s_assertionLogger = std::make_shared<spdlog::logger>("ASSERT", sinks.begin(), sinks.end());
			s_assertionLogger->set_level(spdlog::level::trace);
			break;
		}
		}
	}
}