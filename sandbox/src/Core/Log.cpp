#include "pch.h"
#include <SDL/SDL.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include "Core/Log.h"

namespace Sandbox
{
	std::string LogSDLError(std::string str)
	{
		return std::string("SDL error: " + str + " " + SDL_GetError());
	}

	sptr<spdlog::logger> Log::m_logger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		std::vector<spdlog::sink_ptr> sinks;

		sinks.push_back(makesptr<spdlog::sinks::stdout_color_sink_mt>());
		sinks.push_back(makesptr<spdlog::sinks::rotating_file_sink_mt>("logfile.txt", 20000, 3));
		m_logger = makesptr<spdlog::logger>("SANDBOX", begin(sinks), end(sinks));
		m_logger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> Log::GetLogger()
	{
		return m_logger;
	}
}


