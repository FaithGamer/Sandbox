#pragma once

#include <json/json.hpp>
#include "std_macros.h"

namespace Sandbox
{
	using Json = nlohmann::json;

	class Config
	{
	public:
		Config();
		Config(String path);
		

		float GetFloat(String name);
		int GetInt(String name);
		String GetString(String name);
		Config GetConfig(String name);

		template <class T>
		std::vector<T> GetArray(String name);

		Config& operator=(Json json)
		{
			m_json = json;
			return *this;
		}


	private:
		template <class T>
		bool SafeGet(String name, T& value);
		

		String m_path;
		Json m_json;
	};

}