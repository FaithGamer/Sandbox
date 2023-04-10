#pragma once

#include "Sandbox/Input/Input.h"

namespace Sandbox
{
	class Inputs;
	class ButtonInput;

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
		~InputMap();
		ButtonInput* CreateButtonInput(std::string name);
		void DestroyInput(std::string name);
		bool OnEvent(const SDL_Event& e);
		void OnInputEventModified(Input* input);
		Input* GetInput(std::string name);
		std::unordered_map<std::string, Input*> GetInputs();
		std::string GetName() const;

	private:
		friend Inputs;
		InputMap(std::string name);
		void UpdateInputsEvents();

		std::vector<std::vector<Input*>> m_byEvents;
		std::unordered_map<std::string, Input*> m_byNames;
		std::vector<Input*> m_modified;
		std::vector<Input*> m_toDelete;

		bool m_mustUpdate;

		std::string m_name;
	};
}