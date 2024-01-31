#pragma once

#include <json/json.hpp>
#include "std_macros.h"

namespace Sandbox
{
	using Json = nlohmann::json;

	class Serializable
	{
	public:
		virtual ~Serializable() {};
		virtual Json ToJson() = 0;
	};
	class Config : Serializable
	{
	public:
		Config();
		Config(String path);
		

		float GetFloat(String name);
		int GetInt(String name);
		bool GetBool(String name);
		String GetString(String name);
		Config GetConfig(String name);
		String GetPath() const;
		Json ToJson() override;

		template <class T>
		std::vector<T> GetArray(String name);

		inline Config& operator=(Json json)
		{
			m_json = json;
			return *this;
		}
		inline operator Json()
		{
			return m_json;
		}

	private:
		template <class T>
		bool SafeGet(String name, T& value);
		

		String m_path;
		Json m_json;
	};

}