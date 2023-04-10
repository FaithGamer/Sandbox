#pragma once
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Singleton.h"

namespace Sandbox
{

	class Inputs : public Singleton<Inputs>
	{
	public:
		Inputs();
		static InputMap& CreateInputMap();
		static InputMap& CreateInputMap(std::string name);
		static void DestroyInputMap(std::string name);
		static void SetCurrentInputMap(std::string name);
		static std::string GetCurrentInputMapName();
		static InputMap& GetCurrentInputMap();
		static InputMap* GetInputMap();
		static InputMap* GetInputMap(std::string name);
		static std::vector<std::string> GetInputMapNameList();
		static bool HasInputMap();

	private:
		friend Engine;
		std::unordered_map<std::string, InputMap> m_inputsMap;
		InputMap* m_current;
	};
}
