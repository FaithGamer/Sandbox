#pragma once

#include "Sandbox/Input/Input.h"

namespace Sandbox
{
	class Inputs;
	class ButtonInput;
	struct InputMapContainer;

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
		~InputMap();
		/// @brief Create a ButtonInput 
		/// @param name input name
		/// @return shared_ptr to the created input
		sptr<ButtonInput> CreateButtonInput(std::string name);
		/// @brief Set wether or not this InputMap is used by the InputSystem
		/// @param active 
		void SetActive(bool active);
		/// @brief Delete an input
		/// @param name Name of the Input to delete
		void DestroyInput(std::string name);

		bool OnEvent(const SDL_Event& e);
		void OnInputEventModified(sptr<Input> input);

		/// @brief Should this input map be used or not.
		bool IsActive() const;
		/// @brief Get an input according to it's name
		sptr<Input> GetInput(std::string name);
		/// @brief Get the whole input map
		std::unordered_map<std::string, sptr<Input>> GetInputs();
		/// @brief Get the name of this InputMap
		std::string GetName() const;

	private:
		void UpdateInputsEvents();

		std::vector<std::vector<sptr<Input>>> m_byEvents;
		std::unordered_map<std::string, sptr<Input>> m_byNames;
		std::vector<sptr<Input>> m_modified;
		std::vector<sptr<Input>> m_toDelete;

		bool m_mustUpdate;
		bool m_isActive;

		std::string m_name;
	};
}