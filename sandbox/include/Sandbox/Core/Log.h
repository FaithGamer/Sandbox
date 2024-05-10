#pragma once
#include <string>
#include <spdlog/spdlog.h>

#include "Sandbox/Internal/Singleton.h"
#include "Sandbox/Core/std_macros.h"


namespace Sandbox
{
	std::string LogSDLError(std::string str);

	class Log : public Singleton<Log>
	{

	public:
		void Init(bool logging);
		sptr<spdlog::logger> GetLogger();
	private:
		friend sptr<Log> Singleton<Log>::Instance();
		friend void Singleton<Log>::Kill();
		Log() = default;

		sptr<spdlog::logger> m_logger = nullptr;
	};
}
#ifndef SANDBOX_NO_LOGGING
#define ASSERT_LOG_ERROR(condition, ...) if(!condition){Log::Instance()->GetLogger()->error(__VA_ARGS__); assert(condition); Log::Instance()->GetLogger()->flush();}

#define LOG_ERROR(...) Log::Instance()->GetLogger()->error(__VA_ARGS__); Log::Instance()->GetLogger()->flush();
#define LOG_WARN(...) Log::Instance()->GetLogger()->warn(__VA_ARGS__); Log::Instance()->GetLogger()->flush();
#define LOG_INFO(...) Log::Instance()->GetLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...) Log::Instance()->GetLogger()->trace(__VA_ARGS__)
#else
#define ASSERT_LOG_ERROR(...)
#define LOG_ERROR(...) 
#define LOG_WARN(...)
#define LOG_INFO(...) 
#define LOG_TRACE(...) 
#endif

