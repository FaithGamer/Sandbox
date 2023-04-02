#include "pch.h"
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Vector.h"

namespace Sandbox
{
	InputMap::InputMap(std::string name) : m_name(name)
	{
		m_byEvents.resize((int)EventType::EventTypeCount);
	}

	bool InputMap::OnEvent(const SDL_Event& e)
	{
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
				if(input->MouseButtonPressed(e))
					eventHandled = true;
			}
			break;
		case SDL_MOUSEMOTION:
			for (auto& input : m_byEvents[(int)EventType::MouseMove])
			{
				if(input->MouseMoved(e))
					eventHandled = true;
			}
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			for (auto& input : m_byEvents[(int)EventType::ControllerBtn])
			{
				if(input->ControllerButtonPressed(e))
					eventHandled = true;
			}
			break;
		case SDL_CONTROLLERBUTTONUP:
			for (auto& input : m_byEvents[(int)EventType::ControllerBtn])
			{
				if(input->ControllerButtonReleased(e))
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
					if(input->ControllerStickMoved(e))
						eventHandled = true;
				}
			}
			else
			{
				for (auto& input : m_byEvents[(int)EventType::ControllerStick])
				{
					if(input->ControllerTriggerMoved(e))
						eventHandled = true;
				}
			}
			break;
		case SDL_TEXTINPUT:
			break;

		case SDL_TEXTEDITING:
			break;
		}
		return eventHandled;
	}

	void InputMap::OnInputEventModified(sptr<Input> input)
	{
		m_modifiedInputs.emplace_back(input);
	}

	void InputMap::AddInput(sptr<Input> input)
	{
		//Add to collection
		std::string name = input->GetName();
		auto input_it = m_byNames.find(name);
		if (input_it == m_byNames.end())
		{
			m_modifiedInputs.push_back(input);
			m_byNames.insert(std::make_pair(name, input));
			input->m_inputMap = this;
		}
		else
		{
			LOG_WARN("Trying to add Input with already existing name \"" + input->GetName() + "\" already exist in the InputMap \"" + GetName()
				+ "\". No input has been added.");
		}
	}

	void InputMap::RemoveInput(sptr<Input> input)
	{
		LOG_ERROR("Cannot remove Input :o)");
	}

	sptr<Input> InputMap::GetInput(std::string name)
	{
		auto input_it = m_byNames.find(name);
		if (input_it == m_byNames.end())
		{
			LOG_ERROR("Can't find the input \"" + name + "\" in the InputMap \"" + GetName() + "\"");
			return nullptr;
		}
		return input_it->second;
	}

	std::string InputMap::GetName() const
	{
		return m_name;
	}

	void InputMap::UpdateInputsEvents()
	{
		for (auto& input : m_modifiedInputs)
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
		m_modifiedInputs.clear();
	}
}