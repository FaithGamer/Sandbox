#include "pch.h"
#include "Sandbox/Json.h"
#include "Sandbox/Log.h"

namespace Sandbox
{
	Config::Config()
	{

	}

	Config::Config(String path) : m_path(path)
	{
		auto file = std::ifstream(path);
		try {

			file >> m_json;
		}
		catch (Json::exception& exception)
		{
			String errorMsg = "Config error when parsing: " + path + "\n";
			errorMsg += "json exception: " + std::to_string(exception.id) + ", ";
			errorMsg += *exception.what() + "\n";

			LOG_ERROR(errorMsg);
		}
	}
	float Config::GetFloat(String name)
	{
		float value;
		if (SafeGet<float>(name.c_str(), value))
		{
			return value;
		}
		else
		{
			return 0.0f;
		}
	}

	int Config::GetInt(String name)
	{
		int value;
		if (SafeGet<int>(name, value))
		{
			return value;
		}
		else
		{
			return 0;
		}
	}

	String Config::GetString(String name)
	{
		String  value;
		if (SafeGet<String>(name, value))
		{
			return value;
		}
		else
		{
			return "undefined";
		}
	}

	Config Config::GetConfig(String name)
	{
		Config value;
		if (SafeGet<Config>(name, value))
		{
			value.m_path = m_path + "->" + name;
			return value;
		}
		else
		{
			return Config();
		}
	}

	template <class T>
	std::vector<T> Config::GetArray(String name)
	{
		std::vector<T> value;
		if (SafeGet<std::vector<T>>(name, value))
		{
			return value;
		}
		else
		{
			return std::vector<T>();
		}
	}

	template <class T>
	bool Config::SafeGet(String name, T& value)
	{
		try {
			value = m_json.at(name);
			return true;
		}
		catch (Json::exception& exception)
		{
			String errorMsg = "Config error in file " + m_path + ", parameter: " + String(name) + "\n";
			errorMsg += "json exception: " + std::to_string(exception.id) + ", ";
			errorMsg += String(exception.what()) + "\n";

			LOG_ERROR(errorMsg);

			return false;
		}
	}
}