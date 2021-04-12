#pragma once
#include <memory>
#include "spdlog/spdlog.h"
#include "Engine_Macros.h"

namespace Pale {
	class PALE_API Log_System {

	public:
		Log_System() = default;
		~Log_System() = default;

		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#define LOG_SYSTEM_INIT ::Pale::Log_System::Init()

#define PALE_ENGINE_TRACE(...) ::Pale::Log_System::GetCoreLogger()->trace(__VA_ARGS__)
#define PALE_ENGINE_INFO(...) ::Pale::Log_System::GetCoreLogger()->info(__VA_ARGS__)
#define PALE_ENGINE_WARN(...) ::Pale::Log_System::GetCoreLogger()->warn(__VA_ARGS__)
#define PALE_ENGINE_ERROR(...) ::Pale::Log_System::GetCoreLogger()->error(__VA_ARGS__)

#define PALE_TRACE(...) ::Pale::Log_System::GetLientLogger()->trace(__VA_ARGS__)
#define PALE_INFO(...) ::Pale::Log_System::GetClientLogger()->info(__VA_ARGS__)
#define PALE_WARN(...) ::Pale::Log_System::GetClientLogger()->warn(__VA_ARGS__)
#define PALE_ERROR(...) ::Pale::Log_System::GetClientLogger()->error(__VA_ARGS__)