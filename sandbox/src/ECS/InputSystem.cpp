#include "pch.h"

#include "Sandbox/Input/Inputs.h"
#include "Sandbox/ECS/InputSystem.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Input/DirectionalInput.h"
#include "Sandbox/Core/Log.h"

namespace Sandbox
{
	InputSystem::InputSystem()
	{
		SetPriority(-10000);
	}

	bool InputSystem::OnEvent(SDL_Event& event)
	{
		bool eventConsumed = false;
		if (m_rebind != nullptr)
		{
			//Rebinding is occuring
			Rebind(event);
		}
		else
		{
			//Normal usage of inputs
			for (auto& inputMap : Inputs::GetInputMaps())
			{
				if (!inputMap->IsActive())
					continue;

				eventConsumed = inputMap->OnEvent(event);
			}
		}
		return eventConsumed;
	}

	void InputSystem::StartRebind(sptr<Input> input, int peripherals, int version)
	{
		m_rebind = input;
		m_rebindVersion = version;
		m_rebindPeripherals = peripherals;

		if (m_rebindPeripherals == 0)
		{
			LOG_WARN("Rebind peripheral flag no set.");
		}
	}

	void InputSystem::EndRebind()
	{
		m_rebind = nullptr;
		m_rebindVersion = 0;
		m_rebindPeripherals = 0;
	}

	int InputSystem::GetUsedMethod()
	{
		return System::Method::Event;
	}

	bool InputSystem::IsRebinding()
	{
		return !(m_rebind == nullptr);
	}
	void InputSystem::Rebind(SDL_Event& e)
	{
		switch (m_rebind->GetType())
		{
		case InputType::Button:
		{
			auto buttonInput = static_pointer_cast<ButtonInput>(m_rebind);

			if (m_rebindVersion >= buttonInput->GetBindingsCount())
			{
				LOG_WARN("InputSystem::Rebind: Button binding version {0} does not exists in input {1}.", m_rebindVersion, buttonInput->GetName());
				return;
			}

			if ((m_rebindPeripherals & PeripheralFlag::Mouse) == PeripheralFlag::Mouse)
			{
				//Mouse
				switch (e.type)
				{
				case SDL_MOUSEBUTTONUP:
					if (m_rebindVersion == -1)
					{
						buttonInput->AddMouse((MouseButton)e.button.button);
						EndRebind();
					}
					else
					{
						buttonInput->SetMouse(m_rebindVersion, (MouseButton)e.button.button);
						EndRebind();
					}
					break;
				}
			}

			if ((m_rebindPeripherals & PeripheralFlag::Keyboard) == PeripheralFlag::Keyboard)
			{
				//Keyboard
				switch (e.type)
				{
				case SDL_KEYUP:
					if (m_rebindVersion == -1)
					{
						buttonInput->AddKey((KeyScancode)e.key.keysym.scancode);
						EndRebind();
					}
					else
					{
						buttonInput->SetKey(m_rebindVersion, (KeyScancode)e.key.keysym.scancode);
						EndRebind();
					}
					break;
				}
			}

			if ((m_rebindPeripherals & PeripheralFlag::Controller) == PeripheralFlag::Controller)
			{
				//Controller
				switch (e.type)
				{
				case SDL_CONTROLLERBUTTONUP:
					if (m_rebindVersion == -1)
					{
						buttonInput->AddControllerButton((ControllerButton)e.cbutton.button);
						EndRebind();
					}
					else
					{
						buttonInput->SetControllerButton(m_rebindVersion, (ControllerButton)e.cbutton.button);
						EndRebind();
					}
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
					if (m_rebindVersion == -1)
					{
						buttonInput->AddControllerTrigger(ControllerTrigger::Left);
						EndRebind();
					}
					else
					{
						buttonInput->SetControllerTrigger(m_rebindVersion, ControllerTrigger::Left);
						EndRebind();
					}
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
					if (m_rebindVersion == -1)
					{
						buttonInput->AddControllerTrigger(ControllerTrigger::Right);
					}
					else
					{
						buttonInput->SetControllerTrigger(m_rebindVersion, ControllerTrigger::Right);
					}
					break;
				}
			}
		}
		break;
		default:
			LOG_WARN("Rebind of input type {0} not implemented", InputTypeName(m_rebind->GetType()));
			break;
		}
	}
}