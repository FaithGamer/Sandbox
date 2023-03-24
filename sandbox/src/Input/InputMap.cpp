#include "pch.h"
#include "InputMap.h"
#include "Containers/Vector.h"

namespace Sandbox
{
	InputMap::InputMap(std::string name) : m_name(name)
	{
		m_byEvents.resize((int)EventType::EventTypeCount);
	}

	void InputMap::Update(const SDL_Event& e)
	{
		UpdateInputEvent();
		switch (e.type)
		{
		case SDL_KEYDOWN:
			for (auto& input : m_byEvents[(int)EventType::Key])
			{
				input->KeyPressed(e);
			}
			break;
		case SDL_KEYUP:
			for (auto& input : m_byEvents[(int)EventType::Key])
			{
				input->KeyReleased(e);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			for (auto& input : m_byEvents[(int)EventType::MouseBtn])
			{
				input->MouseButtonPressed(e);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			for (auto& input : m_byEvents[(int)EventType::MouseBtn])
			{
				input->MouseButtonPressed(e);
			}
			break;
		case SDL_MOUSEMOTION:
			for (auto& input : m_byEvents[(int)EventType::MouseMove])
			{
				input->MouseMoved(e);
			}
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			for (auto& input : m_byEvents[(int)EventType::ControllerBtn])
			{
				input->ControllerButtonPressed(e);
			}
			break;
		case SDL_CONTROLLERBUTTONUP:
			for (auto& input : m_byEvents[(int)EventType::ControllerBtn])
			{
				input->ControllerButtonReleased(e);
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
					input->ControllerStickMoved(e);
				}
			}
			else
			{
				for (auto& input : m_byEvents[(int)EventType::ControllerStick])
				{
					input->ControllerTriggerMoved(e);
				}
			}
			break;
		case SDL_TEXTINPUT:
			break;

		case SDL_TEXTEDITING:
			break;
		}
	}

	void InputMap::UpdateInputEvent(sptr<Input> input)
	{
		m_updated.emplace_back(input);
	}

	void InputMap::AddInput(sptr<Input> input)
	{
		//Add to collection
		std::string name = input->GetName();
		auto input_it = m_byNames.find(name);
		if (input_it == m_byNames.end())
		{
			m_updated.push_back(input);
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

	std::string InputMap::GetName()
	{
		return m_name;
	}

	void InputMap::UpdateInputEvent()
	{
		for (auto& input : m_updated)
		{
			const InputEvent& events = input->m_eventsListened;
			const std::string& name = input->GetName();

			if (events.keyButton && !Vector::Contains(input, m_byEvents[(int)EventType::Key]))
			{
				m_byEvents[(int)EventType::Key].push_back(input);
			}
			else if(!events.keyButton)
			{
				Vector::Remove(input, m_byEvents[(int)EventType::Key]);
			}

			if (events.keyText && !Vector::Contains(input, m_byEvents[(int)EventType::Text]))
			{
				m_byEvents[(int)EventType::Text].push_back(input);
			}
			else if(!events.keyText)
			{
				Vector::Remove(input, m_byEvents[(int)EventType::Text]);
			}

			if (events.mouseButton && !Vector::Contains(input, m_byEvents[(int)EventType::MouseBtn]))
			{
				m_byEvents[(int)EventType::MouseBtn].push_back(input);
			}
			else if(!events.mouseButton)
			{
				Vector::Remove(input, m_byEvents[(int)EventType::MouseBtn]);
			}

			if (events.mouseMovement && !Vector::Contains(input, m_byEvents[(int)EventType::MouseMove]))
			{
				m_byEvents[(int)EventType::MouseMove].push_back(input);
			}
			else if(!events.mouseMovement)
			{
				Vector::Remove(input, m_byEvents[(int)EventType::MouseMove]);
			}

			if (events.controllerButton && !Vector::Contains(input, m_byEvents[(int)EventType::ControllerBtn]))
			{
				m_byEvents[(int)EventType::ControllerBtn].push_back(input);
			}
			else if(!events.controllerButton)
			{
				Vector::Remove(input, m_byEvents[(int)EventType::ControllerBtn]);
			}

			if (events.controllerStick && !Vector::Contains(input, m_byEvents[(int)EventType::ControllerStick]))
			{
				m_byEvents[(int)EventType::ControllerStick].push_back(input);
			}
			else if(!events.controllerStick)
			{
				Vector::Remove(input, m_byEvents[(int)EventType::ControllerStick]);
			}

			if (events.controllerTrigger && !Vector::Contains(input, m_byEvents[(int)EventType::ControllerTrigger]))
			{
				m_byEvents[(int)EventType::ControllerTrigger].push_back(input);
			}
			else if(!events.controllerTrigger)
			{
				Vector::Remove(input, m_byEvents[(int)EventType::ControllerTrigger]);
			}
		}
		m_updated.clear();
	}
}