#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

enum class LOGGING_METHOD {
	BASIC = 0,
	FILE_AND_TERMINAL,
	FILE_ONLY,
	TERMINAL_ONLY
};

namespace Pale {
	class Log_System {

	public:
		Log_System() = default;
		~Log_System() = default;

		static void Init(LOGGING_METHOD methodOfLogging);

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_clientLogger; }
		inline static std::shared_ptr<spdlog::logger> GetTestsLogger() { return s_testsLogger; }
		inline static std::shared_ptr<spdlog::logger> GetAssertionLogger() { return s_assertionLogger; }
		inline static std::shared_ptr<spdlog::logger> GetAILogger() { return s_aiLogger; }

	private:
		static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> s_consoleSink;
		static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> s_fileSink;
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
		static std::shared_ptr<spdlog::logger> s_testsLogger;
		static std::shared_ptr<spdlog::logger> s_assertionLogger;
		static std::shared_ptr<spdlog::logger> s_aiLogger;
	};
}

#define LOG_SYSTEM_INIT(...) ::Pale::Log_System::Init(__VA_ARGS__)

#define PALE_ENGINE_TRACE(...) ::Pale::Log_System::GetCoreLogger()->trace(__VA_ARGS__)
#define PALE_ENGINE_INFO(...) ::Pale::Log_System::GetCoreLogger()->info(__VA_ARGS__)
#define PALE_ENGINE_WARN(...) ::Pale::Log_System::GetCoreLogger()->warn(__VA_ARGS__)
#define PALE_ENGINE_ERROR(...) { Pale::Log_System::GetCoreLogger()->error(__VA_ARGS__); std::cin.get(); exit(EXIT_FAILURE); }

#define PALE_TRACE(...) ::Pale::Log_System::GetClientLogger()->trace(__VA_ARGS__)
#define PALE_INFO(...) ::Pale::Log_System::GetClientLogger()->info(__VA_ARGS__)
#define PALE_WARN(...) ::Pale::Log_System::GetClientLogger()->warn(__VA_ARGS__)
#define PALE_ERROR(...) { Pale::Log_System::GetClientLogger()->error(__VA_ARGS__); std::cin.get(); exit(EXIT_FAILURE); }

#define PALE_TESTS_TRACE(...) ::Pale::Log_System::GetTestsLogger()->trace(__VA_ARGS__)
#define PALE_TESTS_INFO(...) ::Pale::Log_System::GetTestsLogger()->info(__VA_ARGS__)
#define PALE_TESTS_WARN(...) ::Pale::Log_System::GetTestsLogger()->warn(__VA_ARGS__)
#define PALE_TESTS_ERROR(...) { Pale::Log_System::GetTestsLogger()->error(__VA_ARGS__); std::cin.get(); exit(EXIT_FAILURE); }

#define PALE_ASSERTION_TRACE(...) ::Pale::Log_System::GetAssertionLogger()->trace(__VA_ARGS__)
#define PALE_ASSERTION_INFO(...) ::Pale::Log_System::GetAssertionLogger()->info(__VA_ARGS__)
#define PALE_ASSERTION_WARN(...) ::Pale::Log_System::GetAssertionLogger()->warn(__VA_ARGS__)
#define PALE_ASSERTION_ERROR(...) { Pale::Log_System::GetAssertionLogger()->error(__VA_ARGS__); std::cin.get(); exit(EXIT_FAILURE); }

#define PALE_AI_TRACE(...) ::Pale::Log_System::GetAILogger()->trace(__VA_ARGS__)
#define PALE_AI_INFO(...) ::Pale::Log_System::GetAILogger()->info(__VA_ARGS__)
#define PALE_AI_WARN(...) ::Pale::Log_System::GetAILogger()->warn(__VA_ARGS__)
#define PALE_AI_ERROR(...) { Pale::Log_System::GetAILogger()->error(__VA_ARGS__); std::cin.get(); exit(EXIT_FAILURE); }