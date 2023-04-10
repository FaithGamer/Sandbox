#include "pch.h"

#include "Sandbox/Log.h"
#include "Sandbox/Input/Inputs.h"

namespace Sandbox
{
	Inputs::Inputs() : m_current(nullptr)
	{

	}

	InputMap& Inputs::CreateInputMap()
	{
		auto instance = Inputs::Instance();
		InputMap inputMap("InputMap_"+std::to_string(instance->m_inputsMap.size()));
		if (!instance->m_inputsMap.insert(std::make_pair(inputMap.GetName(), inputMap)).second)
		{
			LOG_ERROR("Couldn't insert InputMap");
		}
		if (instance->m_current == nullptr)
		{
			instance->m_current = &instance->m_inputsMap.at(inputMap.GetName());
		}

		return instance->m_inputsMap.at(inputMap.GetName());
	}

	InputMap& Inputs::CreateInputMap(std::string name)
	{
		auto instance = Inputs::Instance();
		InputMap inputMap(name);
		int i = 1;
		while (!instance->m_inputsMap.insert(std::make_pair(inputMap.GetName(), inputMap)).second)
		{
			LOG_WARN("There is already an input map with the name: " + inputMap.GetName());
			inputMap.m_name = name + std::to_string(i++);
			LOG_WARN("Input Map renamed: " + inputMap.GetName());
		}
		if (instance->m_current == nullptr)
		{
			instance->m_current = &instance->m_inputsMap.at(inputMap.GetName());
		}
		return instance->m_inputsMap.at(inputMap.GetName());
	}

	void Inputs::DestroyInputMap(std::string name)
	{
		auto instance = Inputs::Instance();
		if (instance->m_inputsMap.erase(name) < 1)
		{
			LOG_WARN("No input map with the name: " + name + " exists. No input map has been removed.");
		}
	}

	void Inputs::SetCurrentInputMap(std::string name)
	{
		auto instance = Inputs::Instance();
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
		auto instance = Inputs::Instance();
		return instance->m_current->GetName();
	}

	/// @brief Access the current InputMap
	/// Undefined behaviour if there is no input map created.
	/// @return Referece ot the current InputMap
	InputMap& Inputs::GetCurrentInputMap()
	{
		auto instance = Inputs::Instance();
		return *instance->m_current;
	}

	bool Inputs::HasInputMap()
	{
		if (Inputs::Instance()->m_current != nullptr)
			return true;
		else
			return false;
	}

	InputMap* Inputs::GetInputMap()
	{
		if (Inputs::Instance()->m_inputsMap.empty())
		{
			LOG_ERROR("No input map. Empty map returned.");
			return nullptr;
		}
		else
		{
			return &Inputs::Instance()->m_inputsMap.begin()->second;
		}
	}

	InputMap* Inputs::GetInputMap(std::string name)
	{
		auto instance = Inputs::Instance();
		auto found = instance->m_inputsMap.find(name);
		if (found == instance->m_inputsMap.end())
		{
			LOG_ERROR("No input map with the name: " + name + " exists. Empty map returned.");
			return nullptr;
		}
		return &found->second;
	}

	std::vector<std::string> Inputs::GetInputMapNameList()
	{
		auto instance = Inputs::Instance();

		std::vector<std::string> names;
		for (auto& kvp : instance->m_inputsMap)
		{
			names.emplace_back(kvp.first);
		}
		return names;
	}
}