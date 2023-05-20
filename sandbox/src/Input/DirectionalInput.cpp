#include "pch.h"
#include "Sandbox/Input/DirectionalInput.h"
#include "Sandbox/Vector.h"

namespace Sandbox
{
	DirectionalInput::DirectionalInput(std::string name) : m_name(name), m_triggerDeadzone(0.1f), m_stickDeadzone(0.1f)
	{
	}

	void DirectionalInput::ListenEventAndBind(const SDL_Event& e, int version)
	{
		//To do
	}

	void DirectionalInput::SetBindings(const DirectionalBindings& bindings)
	{
		m_bindings = bindings;
	}

	std::string DirectionalInput::GetName() const
	{
		return m_name;
	}

	InputType DirectionalInput::GetType() const
	{
		return InputType::Directional;
	}

	void DirectionalInput::AddStick(ControllerStick stick)
	{
		m_bindings.directions.push_back(Direction(stick));
	}

	void DirectionalInput::AddButtons(std::vector<DirectionalButton> buttons)
	{
		m_bindings.directions.push_back(Direction(buttons));
	}

	void DirectionalInput::SetStick(ControllerStick stick, int version)
	{
		if (m_bindings.directions.size() <= version)
		{
			LOG_WARN("DirectionalInput::SetStick -> version does not exists.");
			return;
		}
		m_bindings.directions[version].stick = stick;
	}

	void DirectionalInput::SetButtons(std::vector<DirectionalButton> buttons, int version)
	{
		if (m_bindings.directions.size() <= version)
		{
			LOG_WARN("DirectionalInput::SetButtons -> version does not exists.");
			return;
		}
		m_bindings.directions[version].buttons = buttons;
	}

	void DirectionalInput::RemoveBinding(int version)
	{
		if (!Vector::RemoveAt(m_bindings.directions, version))
		{
			LOG_WARN("DirectionalInput::RemoveBinding -> version does not exists.");
		}
	}

	void DirectionalInput::SetStickDeadZone(float deadzone)
	{
		m_stickDeadzone = deadzone;
	}

	void DirectionalInput::SetTriggerDeadZone(float deadzone)
	{
		m_triggerDeadzone = deadzone;
	}

	bool DirectionalInput::KeyPressed(const SDL_Event& e)
	{
		bool mustComputeState = false;
		for (auto& direction : m_bindings.directions)
		{
			for (auto& button : direction.buttons)
			{
				if (e.key.keysym.scancode == (SDL_Scancode)button.button.key && !button.pressed)
				{
					button.pressed = true;
					mustComputeState = true;
				}

			}
		}
		if (mustComputeState)
		{
			ComputeState();
			return true;
		}
		return false;
	}

	bool DirectionalInput::KeyReleased(const SDL_Event& e)
	{
		return false;
	}
	bool DirectionalInput::MouseButtonPressed(const SDL_Event& e)
	{
		return false;
	}
	bool DirectionalInput::MouseButtonReleased(const SDL_Event& e)
	{
		return false;
	}
	bool DirectionalInput::ControllerButtonPressed(const SDL_Event& e)
	{
		return false;
	}
	bool DirectionalInput::ControllerButtonReleased(const SDL_Event& e)
	{
		return false;
	}
	bool DirectionalInput::ControllerTriggerMoved(const SDL_Event& e)
	{
		return false;
	}
	bool DirectionalInput::ControllerStickMoved(const SDL_Event& e)
	{
		return false;
	}

	void DirectionalInput::UpdateEventListened()
	{

	}

	DirectionalBindings& DirectionalInput::GetBindings()
	{
		return m_bindings;
	}
}