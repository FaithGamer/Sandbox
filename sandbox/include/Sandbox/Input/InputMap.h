#pragma once

#include "Sandbox/Input/Input.h"

namespace Sandbox
{
	enum class EventType : int
	{
		Key = 0,
		Text,
		MouseBtn,
		MouseMove,
		ControllerBtn,
		ControllerStick,
		ControllerTrigger,
		EventTypeCount
	};

	//To do: enable serialization/deserialization
	class InputMap
	{
	public:
		InputMap(std::string name);
		bool OnEvent(const SDL_Event& e);
		void OnInputEventModified(sptr<Input> input);
		void AddInput(sptr<Input> input);
		void RemoveInput(sptr<Input> input);
		sptr<Input> GetInput(std::string name);
		std::string GetName() const;

	private:
		void UpdateInputsEvents();

		//Inputs sorted by the events they listen to.
		std::vector<std::vector<sptr<Input>>> m_byEvents;

		//Inputs sorted by their names
		std::unordered_map<std::string, sptr<Input>> m_byNames;

		//Update listening 
		std::vector<sptr<Input>> m_modifiedInputs;

		std::string m_name;
	};
}