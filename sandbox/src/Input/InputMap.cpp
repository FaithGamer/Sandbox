#include "pch.h"
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Vector.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Input/DirectionalInput.h"

namespace Sandbox
{
	InputMap::InputMap(std::string name) : m_name(name), m_mustUpdate(false), m_isActive(true), m_passThroughImGui(false)
	{
		m_byEvents.resize((int)EventType::EventTypeCount);
	}

	InputMap::~InputMap()
	{

	}

	sptr<ButtonInput> InputMap::CreateButtonInput(std::string name)
	{
		if (CheckHaveInputAndDisplayWarning(name))
		{
			return nullptr;
		}
		auto button = makesptr<ButtonInput>(name);
		AddInput(button);
		return button;
	}

	sptr<DirectionalInput> InputMap::CreateDirectionalInput(std::string name)
	{
		if (CheckHaveInputAndDisplayWarning(name))
		{
			return nullptr;
		}
		auto button = makesptr<DirectionalInput>(name);
		AddInput(button);
		return button;
	}

	void InputMap::SetActive(bool active)
	{
		m_isActive = active;
	}

	void InputMap::SetPassThroughImGui(bool passThrough)
	{
		m_passThroughImGui = passThrough;
	}

	void InputMap::DestroyInput(std::string name)
	{
		auto input = m_byNames.find(name);
		if (input == m_byNames.end())
		{
			LOG_WARN("Cannot destroy input \"" + name + "\", because it doesn't exists.");
		}
		else
		{
			m_modified.erase(input->second);
			m_toDelete.emplace_back(input->second);
			m_mustUpdate = true;
		}
	}

	bool InputMap::OnEvent(const SDL_Event& e, bool handledByImGui)
	{
		if (handledByImGui && !m_passThroughImGui)
			return false;

		if (m_mustUpdate)
			UpdateInputsEvents();

		bool eventHandled = false;
		switch (e.type)
		{
		case SDL_KEYDOWN:
			for (auto& input : m_byEvents[(int)EventType::Key])
			{
				if (input->KeyPressed(e))
					eventHandled = true;
			}
			break;
		case SDL_KEYUP:
			for (auto& input : m_byEvents[(int)EventType::Key])
			{
				if (input->KeyReleased(e))
					eventHandled = true;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			for (auto& input : m_byEvents[(int)EventType::MouseBtn])
			{
				if (input->MouseButtonPressed(e))
					eventHandled = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			for (auto& input : m_byEvents[(int)EventType::MouseBtn])
			{
				if (input->MouseButtonReleased(e))
					eventHandled = true;
			}
			break;
		case SDL_MOUSEMOTION:
			for (auto& input : m_byEvents[(int)EventType::MouseMove])
			{
				if (input->MouseMoved(e))
					eventHandled = true;
			}
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			for (auto& input : m_byEvents[(int)EventType::ControllerBtn])
			{
				if (input->ControllerButtonPressed(e))
					eventHandled = true;
			}
			break;
		case SDL_CONTROLLERBUTTONUP:
			for (auto& input : m_byEvents[(int)EventType::ControllerBtn])
			{
				if (input->ControllerButtonReleased(e))
					eventHandled = true;
			}
			break;
		case SDL_CONTROLLERAXISMOTION:

			if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX
				|| e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY
				|| e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX
				|| e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
			{
				for (auto& input : m_byEvents[(int)EventType::ControllerStick])
				{
					if (input->ControllerStickMoved(e))
						eventHandled = true;
				}
			}
			else
			{
				for (auto& input : m_byEvents[(int)EventType::ControllerStick])
				{
					if (input->ControllerTriggerMoved(e))
						eventHandled = true;
				}
			}
			break;
		case SDL_TEXTINPUT:
			for (auto& input : m_byEvents[(int)EventType::Text])
			{
				if (input->TextEntered(e))
					eventHandled = true;
			}
			break;

		case SDL_TEXTEDITING:
			break;
		}
		return eventHandled;
	}

	void InputMap::OnInputEventModified(sptr<Input> input)
	{
		m_modified.insert(input);
		m_mustUpdate = true;
	}

	bool InputMap::IsActive() const
	{
		return m_isActive;
	}

	sptr<Input> InputMap::GetInput(std::string name)
	{
		auto input_it = m_byNames.find(name);
		if (input_it == m_byNames.end())
		{
			LOG_WARN("Cannot find the input \"" + name + "\" in the InputMap \"" + GetName() + "\"");
			return nullptr;
		}
		return input_it->second;
	}

	std::unordered_map<std::string, sptr<Input>> InputMap::GetInputs()
	{
		return m_byNames;
	}

	std::string InputMap::GetName() const
	{
		return m_name;
	}

	bool InputMap::HaveInput(std::string name) const
	{
		auto input_it = m_byNames.find(name);
		if (input_it == m_byNames.end())
		{
			return false;
		}
		return true;
	}

	void InputMap::UpdateInputsEvents()
	{
		for (auto& input : m_toDelete)
		{
			for (auto& inputs : m_byEvents)
			{
				Vector::Remove(inputs, input);
			}
			m_byNames.erase(input->GetName());
		}
		m_toDelete.clear();
		for (auto& input : m_modified)
		{
			const InputEvent& events = input->m_eventsListened;
			const std::string& name = input->GetName();

			if (events.keyButton && !Vector::Contains(m_byEvents[(int)EventType::Key], input))
			{
				m_byEvents[(int)EventType::Key].push_back(input);
			}
			else if (!events.keyButton)
			{
				Vector::Remove(m_byEvents[(int)EventType::Key], input);
			}

			if (events.keyText && !Vector::Contains(m_byEvents[(int)EventType::Text], input))
			{
				m_byEvents[(int)EventType::Text].push_back(input);
			}
			else if (!events.keyText)
			{
				Vector::Remove(m_byEvents[(int)EventType::Text], input);
			}

			if (events.mouseButton && !Vector::Contains(m_byEvents[(int)EventType::MouseBtn], input))
			{
				m_byEvents[(int)EventType::MouseBtn].push_back(input);
			}
			else if (!events.mouseButton)
			{
				Vector::Remove(m_byEvents[(int)EventType::MouseBtn], input);
			}

			if (events.mouseMovement && !Vector::Contains(m_byEvents[(int)EventType::MouseMove], input))
			{
				m_byEvents[(int)EventType::MouseMove].push_back(input);
			}
			else if (!events.mouseMovement)
			{
				Vector::Remove(m_byEvents[(int)EventType::MouseMove], input);
			}

			if (events.controllerButton && !Vector::Contains(m_byEvents[(int)EventType::ControllerBtn], input))
			{
				m_byEvents[(int)EventType::ControllerBtn].push_back(input);
			}
			else if (!events.controllerButton)
			{
				Vector::Remove(m_byEvents[(int)EventType::ControllerBtn], input);
			}

			if (events.controllerStick && !Vector::Contains(m_byEvents[(int)EventType::ControllerStick], input))
			{
				m_byEvents[(int)EventType::ControllerStick].push_back(input);
			}
			else if (!events.controllerStick)
			{
				Vector::Remove(m_byEvents[(int)EventType::ControllerStick], input);
			}

			if (events.controllerTrigger && !Vector::Contains(m_byEvents[(int)EventType::ControllerTrigger], input))
			{
				m_byEvents[(int)EventType::ControllerTrigger].push_back(input);
			}
			else if (!events.controllerTrigger)
			{
				Vector::Remove(m_byEvents[(int)EventType::ControllerTrigger], input);
			}
		}
		m_modified.clear();
		m_mustUpdate = false;
	}

	
	bool InputMap::CheckHaveInputAndDisplayWarning(std::string name) const
	{
		if (HaveInput(name))
		{
			LOG_WARN("Trying to add Input with already existing name \"" + name + "\" already exist in the InputMap \"" + GetName()
				+ "\". No input has been added.");
			return true;
		}
		return false;
	}

	void InputMap::AddInput(sptr<Input> input)
	{
		m_modified.insert(input);
		m_byNames.insert(std::make_pair(input->GetName(), input));
		input->m_inputMap = this;
		m_mustUpdate = true;
	}
}