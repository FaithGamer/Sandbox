#pragma once
#include "Input/InputMap.h"
#include "Singleton.h"

namespace Sandbox
{

	class Inputs : public Singleton<Inputs>
	{
	public:
		Inputs();
		static void AddInputMap(const InputMap& inputMap);
		static void RemoveInputMap(std::string name);
		static void SetCurrentInputMap(std::string name);
		static std::string GetCurrentInputMapName();
		static InputMap GetCurrentInputMap();
		static InputMap GetInputMap(std::string name);
		static std::vector<std::string> GetInputMapNameList();

	private:

		std::unordered_map<std::string, InputMap> m_inputsMap;
		InputMap* m_current;
	};
}
