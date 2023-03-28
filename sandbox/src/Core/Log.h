#pragma once
#include <string>
#include <spdlog/spdlog.h>

#include "std_macros.h"


namespace Sandbox
{
	std::string LogSDLError(std::string str);

	class Log
	{

	public:
		static void Init();
		static sptr<spdlog::logger> GetLogger();
	private:
		Log();

		static sptr<spdlog::logger> m_logger;
	};
}
#ifdef _CONSOLE
#define ASSERT_LOG_ERROR(condition, ...) if(!condition){Log::GetLogger()->error(__VA_ARGS__); assert(condition);}

#define LOG_ERROR(...) Log::GetLogger()->error(__VA_ARGS__)
#define LOG_WARN(...) Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...) Log::GetLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...) Log::GetLogger()->trace(__VA_ARGS__)
#else
#define ASSERT_LOG_ERROR(...)
#define LOG_ERROR(...) 
#define LOG_WARN(...)
#define LOG_INFO(...) 
#define LOG_TRACE(...) 
#endif

