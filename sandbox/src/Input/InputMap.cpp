#include "pch.h"
#include "InputMap.h"
#include "Tools/Toolbox.h"

namespace Sandbox
{
	InputMap::InputMap(std::string name) : m_name(name)
	{
		m_byEvents.resize(EventType::EventTypeCount);
	}

	void InputMap::Update(const SDL_Event& e)
	{
		Update();
		switch (e.type)
		{
		case SDL_KEYDOWN:
			for (auto& input : m_byEvents[EventType::key])
			{
				input->KeyPressed(e);
			}
			break;
		case SDL_KEYUP:
			for (auto& input : m_byEvents[EventType::key])
			{
				input->KeyReleased(e);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			for (auto& input : m_byEvents[EventType::mouseBtn])
			{
				input->MouseButtonPressed(e);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			for (auto& input : m_byEvents[EventType::mouseBtn])
			{
				input->MouseButtonPressed(e);
			}
			break;
		case SDL_MOUSEMOTION:
			for (auto& input : m_byEvents[EventType::mouseMove])
			{
				input->MouseMoved(e);
			}
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			for (auto& input : m_byEvents[EventType::controllerBtn])
			{
				input->ControllerButtonPressed(e);
			}
			break;
		case SDL_CONTROLLERBUTTONUP:
			for (auto& input : m_byEvents[EventType::controllerBtn])
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
				for (auto& input : m_byEvents[EventType::controllerStick])
				{
					input->ControllerStickMoved(e);
				}
			}
			else
			{
				for (auto& input : m_byEvents[EventType::controllerStick])
				{
					input->ControllerTriggerMoved(e);
				}
			}
			break;
		}
	}

	void InputMap::UpdateInputEvent(sptr<iInput> input)
	{
		m_updated.emplace_back(input);
	}

	void InputMap::AddInput(sptr<iInput> input)
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

	void InputMap::RemoveInput(sptr<iInput> input)
	{
		LOG_ERROR("Cannot remove Input :o)");
	}

	sptr<iInput> InputMap::GetInput(std::string name)
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

	void InputMap::Update()
	{
		for (auto& input : m_updated)
		{
			const InputEvent& events = input->m_eventsListened;
			const std::string& name = input->GetName();

			if (events.keyButton && !Toolbox::VectorContains(input, m_byEvents[EventType::key]))
			{
				m_byEvents[EventType::key].push_back(input);
			}
			else if(!events.keyButton)
			{
				Toolbox::VectorRemove(input, m_byEvents[EventType::key]);
			}

			if (events.keyText && !Toolbox::VectorContains(input, m_byEvents[EventType::text]))
			{
				m_byEvents[EventType::text].push_back(input);
			}
			else if(!events.keyText)
			{
				Toolbox::VectorRemove(input, m_byEvents[EventType::text]);
			}

			if (events.mouseButton && !Toolbox::VectorContains(input, m_byEvents[EventType::mouseBtn]))
			{
				m_byEvents[EventType::mouseBtn].push_back(input);
			}
			else if(!events.mouseButton)
			{
				Toolbox::VectorRemove(input, m_byEvents[EventType::mouseBtn]);
			}

			if (events.mouseMovement && !Toolbox::VectorContains(input, m_byEvents[EventType::mouseMove]))
			{
				m_byEvents[EventType::mouseMove].push_back(input);
			}
			else if(!events.mouseMovement)
			{
				Toolbox::VectorRemove(input, m_byEvents[EventType::mouseMove]);
			}

			if (events.controllerButton && !Toolbox::VectorContains(input, m_byEvents[EventType::controllerBtn]))
			{
				m_byEvents[EventType::controllerBtn].push_back(input);
			}
			else if(!events.controllerButton)
			{
				Toolbox::VectorRemove(input, m_byEvents[EventType::controllerBtn]);
			}

			if (events.controllerStick && !Toolbox::VectorContains(input, m_byEvents[EventType::controllerStick]))
			{
				m_byEvents[EventType::controllerStick].push_back(input);
			}
			else if(!events.controllerStick)
			{
				Toolbox::VectorRemove(input, m_byEvents[EventType::controllerStick]);
			}

			if (events.controllerTrigger && !Toolbox::VectorContains(input, m_byEvents[EventType::controllerTrigger]))
			{
				m_byEvents[EventType::controllerTrigger].push_back(input);
			}
			else if(!events.controllerTrigger)
			{
				Toolbox::VectorRemove(input, m_byEvents[EventType::controllerTrigger]);
			}
		}
		m_updated.clear();
	}
}