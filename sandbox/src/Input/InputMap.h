#pragma once

#include "Input.h"

namespace Sandbox
{
	//Todo: make my own macros for scancode and controller button/trigger/axis
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

	class InputMap
	{
	public:
		InputMap(std::string name);
		void Update(const SDL_Event& e);
		void UpdateInputEvent(sptr<Input> input);
		void AddInput(sptr<Input> input);
		void RemoveInput(sptr<Input> input);
		sptr<Input> GetInput(std::string name);
		std::string GetName();

	private:
		void UpdateInputEvent();

		//Inputs sorted by the events they listen to.
		std::vector<std::vector<sptr<Input>>> m_byEvents;

		//Inputs sorted by their names
		std::unordered_map<std::string, sptr<Input>> m_byNames;

		//Update listening 
		std::vector<sptr<Input>> m_updated;

		std::string m_name;
	};
}