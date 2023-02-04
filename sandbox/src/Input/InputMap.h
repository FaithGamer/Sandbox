#pragma once

#include "iInput.h"

namespace Sandbox
{
	enum EventType : int
	{
		key = 0,
		text,
		mouseBtn,
		mouseMove,
		controllerBtn,
		controllerStick,
		controllerTrigger,
		EventTypeCount
	};

	class InputMap
	{
	public:
		InputMap(std::string name);
		void Update(const SDL_Event& e);
		void UpdateInputEvent(sptr<iInput> input);
		void AddInput(sptr<iInput> input);
		void RemoveInput(sptr<iInput> input);
		sptr<iInput> GetInput(std::string name);
		std::string GetName();

	private:
		void Update();

		//Inputs sorted by the events they listen to.
		std::vector<std::vector<sptr<iInput>>> m_byEvents;

		//Inputs sorted by their names
		std::unordered_map<std::string, sptr<iInput>> m_byNames;

		//Update listening 
		std::vector<sptr<iInput>> m_updated;

		std::string m_name;
	};
}