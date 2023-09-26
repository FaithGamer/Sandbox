#include "pch.h"
#include "Sandbox/Input/DirectionalInput.h"
#include "Sandbox/Vector.h"
#include "Sandbox/Math.h"

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
		if (HaveBinding(stick))
		{
			LOG_WARN("DirectionalInput::AddStick -> bindings already have the stick, no bindings added.");
			return;
		}
		m_bindings.directions.push_back(Direction(stick));
		UpdateEventListened();
	}

	void DirectionalInput::AddButtons(std::vector<DirectionalButton> buttons)
	{
		if (HaveBinding(buttons))
		{
			LOG_WARN("DirectionalInput::AddButtons -> bindings already have one or more of the buttons, no bindings added.");
			return;
		}
		m_bindings.directions.push_back(Direction(buttons));
		UpdateEventListened();
	}

	void DirectionalInput::SetStick(ControllerStick stick, int version)
	{
		if (m_bindings.directions.size() <= version)
		{
			LOG_WARN("DirectionalInput::SetStick -> version does not exists.");
			return;
		}
		m_bindings.directions[version].stick = stick;
		UpdateEventListened();
	}

	void DirectionalInput::SetButtons(std::vector<DirectionalButton> buttons, int version)
	{
		if (m_bindings.directions.size() <= version)
		{
			LOG_WARN("DirectionalInput::SetButtons -> version does not exists.");
			return;
		}
		m_bindings.directions[version].buttons = buttons;
		UpdateEventListened();
	}

	void DirectionalInput::RemoveBinding(int version)
	{
		if (!Vector::RemoveAt(m_bindings.directions, version))
		{
			LOG_WARN("DirectionalInput::RemoveBinding -> version does not exists.");
		}
		UpdateEventListened();
	}

	void DirectionalInput::SetStickDeadZone(float deadzone)
	{
		m_stickDeadzone = deadzone;
	}

	void DirectionalInput::SetTriggerDeadZone(float deadzone)
	{
		m_triggerDeadzone = deadzone;
	}

	bool DirectionalInput::HaveBinding(ControllerStick stick)
	{
		for (auto& direction : m_bindings.directions)
		{
			if (direction.stick.stick == stick)
				return true;
		}
		return false;
	}

	bool DirectionalInput::HaveBinding(std::vector<DirectionalButton> buttons)
	{
		for (auto& direction : m_bindings.directions)
		{
			for (auto& button : direction.buttons)
			{
				for (auto& buttonCompare : buttons)
				{
					if (buttonCompare.button == button.button)
						return true;
				}
			}
		}
		return false;
	}

	bool DirectionalInput::HaveBinding(Button button)
	{
		for (auto& direction : m_bindings.directions)
		{
			for (auto& buttonCompare : direction.buttons)
			{
				if (buttonCompare.button == button)
					return true;
			}
		}
		return false;
	}

	int DirectionalInput::GetBindingsCount() const
	{
		return m_bindings.directions.size();
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
		bool mustComputeState = false;
		for (auto& direction : m_bindings.directions)
		{
			for (auto& button : direction.buttons)
			{
				if (e.key.keysym.scancode == (SDL_Scancode)button.button.key && button.pressed)
				{
					button.pressed = false;
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
	bool DirectionalInput::MouseButtonPressed(const SDL_Event& e)
	{
		bool mustComputeState = false;
		for (auto& direction : m_bindings.directions)
		{
			for (auto& button : direction.buttons)
			{
				if (e.button.button == (Uint8)button.button.mouse && !button.pressed)
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
	bool DirectionalInput::MouseButtonReleased(const SDL_Event& e)
	{
		bool mustComputeState = false;
		for (auto& direction : m_bindings.directions)
		{
			for (auto& button : direction.buttons)
			{
				if (e.button.button == (Uint8)button.button.mouse && button.pressed)
				{
					button.pressed = false;
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
	bool DirectionalInput::ControllerButtonPressed(const SDL_Event& e)
	{
		bool mustComputeState = false;
		for (auto& direction : m_bindings.directions)
		{
			for (auto& button : direction.buttons)
			{
				if ((SDL_GameControllerButton)e.cbutton.button == (SDL_GameControllerButton)button.button.controller && !button.pressed)
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
	bool DirectionalInput::ControllerButtonReleased(const SDL_Event& e)
	{
		bool mustComputeState = false;
		for (auto& direction : m_bindings.directions)
		{
			for (auto& button : direction.buttons)
			{
				if ((SDL_GameControllerButton)e.cbutton.button == (SDL_GameControllerButton)button.button.controller && button.pressed)
				{
					button.pressed = false;
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
	bool DirectionalInput::ControllerTriggerMoved(const SDL_Event& e)
	{
		bool mustComputeState = false;
		for (auto& direction : m_bindings.directions)
		{
			for (auto& button : direction.buttons)
			{
				float value = Math::ScaleRangeTo((float)e.caxis.value, 0.f, 1.f, 0.f, 32767.f);
				float threshold = m_triggerDeadzone;
				if (value >= threshold && button.pressed == false)
				{
					button.pressed = true;
					mustComputeState = true;
				}
				else if (value < threshold && button.pressed == true)
				{
					button.pressed = false;
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

	bool DirectionalInput::ControllerStickMoved(const SDL_Event& e)
	{
		bool mustComputeState = false;
		for (auto& direction : m_bindings.directions)
		{
			if ((SDL_GameControllerAxis)e.caxis.axis == (SDL_GameControllerAxis)direction.stick.stick.xAxis)
			{
				AxisMove((float)e.caxis.value, true, direction);
				return true;
			}
			else if ((SDL_GameControllerAxis)e.caxis.axis == (SDL_GameControllerAxis)direction.stick.stick.yAxis)
			{
				AxisMove((float)e.caxis.value, false, direction);
				return true;
			}
		}
		return false;
	}

	void DirectionalInput::AxisMove(float value, bool x, Direction& direction)
	{
		value = Math::ScaleRangeTo(value, -1.0f, 1.0f, -32768.f, 32767.f);
		float absValue = std::fabs(value);
		if (absValue > m_stickDeadzone)
		{
			absValue = Math::ScaleRangeTo(absValue, m_stickDeadzone, 1.0f, 0.0f, 1.0f);

			if (value < 0.f)
				value = -absValue;
			else
				value = absValue;

			if (x)
				direction.stick.currentDirection.x = value;
			else
				direction.stick.currentDirection.y = value;

			ComputeState();
		}
	}

	void DirectionalInput::UpdateEventListened()
	{
		InputEvent listened;

		for (auto& direction : m_bindings.directions)
		{
			if (direction.stick.stick != ControllerStick::None)
			{
				listened.controllerStick = true;
			}
			for (auto& button : direction.buttons)
			{
				if (button.button.key != KeyScancode::Unknown)
				{
					listened.keyButton = true;
				}
				if (button.button.mouse != MouseButton::Invalid)
				{
					listened.mouseButton = true;
				}
				if (button.button.controller != ControllerButton::Invalid)
				{
					listened.controllerButton = true;
				}
				if (button.button.trigger != ControllerTrigger::Undefined)
				{
					listened.controllerTrigger = true;
				}
			}
		}
		if (m_eventsListened != listened)
		{
			m_eventsListened = listened;
			OnEventListenedUpdated();
		}
	}

	DirectionalBindings& DirectionalInput::GetBindings()
	{
		return m_bindings;
	}

	void DirectionalInput::ComputeState()
	{
		Vec2f sum(0, 0);

		for (auto& direction : m_bindings.directions)
		{
			for (auto& button : direction.buttons)
			{
				if (button.pressed)
				{
					sum += button.direction;
				}
			}
			if (direction.stick.stick != ControllerStick::None)
			{
				sum += direction.stick.currentDirection;
			}
		}

		if (m_state.direction != sum)
		{
			m_state.direction = sum;
			signal.SendSignal(&m_state);
		}
	}

}