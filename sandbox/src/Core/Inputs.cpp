#include "pch.h"
#include "Inputs.h"
#include "Log.h"
namespace Sandbox
{
	Inputs::Inputs() : m_current(nullptr)
	{

	}
	void Inputs::AddInputMap(const InputMap& inputMap)
	{
		auto instance = Inputs::Get();
		if (!instance->m_inputsMap.insert(std::make_pair(inputMap.GetName(), inputMap)).second)
		{
			LOG_WARN("There is already an input map with the name: " + inputMap.GetName() + ". No input map has been inserted.");
		}
		if (instance->m_current == nullptr)
		{
			instance->m_current = &instance->m_inputsMap.at(inputMap.GetName());
		}
	}

	void Inputs::RemoveInputMap(std::string name)
	{
		auto instance = Inputs::Get();
		if (instance->m_inputsMap.erase(name) < 1)
		{
			LOG_WARN("No input map with the name: " + name + " exists. No input map has been removed.");
		}
	}

	void Inputs::SetCurrentInputMap(std::string name)
	{
		auto instance = Inputs::Get();
		auto found = instance->m_inputsMap.find(name);
		if (found == instance->m_inputsMap.end())
		{
			LOG_ERROR("No input map with the name: " + name + " exists. Couldn't change the current input map.");
		}
		else
		{
			instance->m_current = &found->second;
		}
	}

	std::string Inputs::GetCurrentInputMapName()
	{
		auto instance = Inputs::Get();
		return instance->m_current->GetName();
	}

	InputMap Inputs::GetCurrentInputMap()
	{
		auto instance = Inputs::Get();
		return *instance->m_current;
	}

	InputMap Inputs::GetInputMap(std::string name)
	{
		auto instance = Inputs::Get();
		auto found = instance->m_inputsMap.find(name);
		if (found == instance->m_inputsMap.end())
		{
			LOG_ERROR("No input map with the name: " + name + " exists. Empty map returned.");
			return InputMap("Empty");
		}
		return found->second;
	}

	std::vector<std::string> Inputs::GetInputMapNameList()
	{
		auto instance = Inputs::Get();

		std::vector<std::string> names;
		for (auto kvp : instance->m_inputsMap)
		{
			names.emplace_back(kvp.first);
		}
		return names;
	}
}