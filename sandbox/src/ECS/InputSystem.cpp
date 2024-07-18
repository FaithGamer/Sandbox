#include "pch.h"

#include "Sandbox/Input/Inputs.h"
#include "Sandbox/ECS/InputSystem.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Input/DirectionalInput.h"
#include "Sandbox/Core/Log.h"
#include "Sandbox/Core/Container.h"

namespace Sandbox
{
	InputSystem::InputSystem()
	{
		SetPriority(-10000);
	}

	void InputSystem::OnStart()
	{
		//Open one game controller

		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			InitController(i);
		}
	}

	bool InputSystem::OnEvent(SDL_Event& event)
	{
		bool eventConsumed = false;
		if (m_rebind != nullptr)
		{
			//Rebinding is occuring
			eventConsumed = Rebind(event);
		}
		if(!eventConsumed)
		{
			//Normal usage of inputs
			for (auto& inputMap : Inputs::GetInputMaps())
			{
				if (!inputMap->IsActive())
					continue;

				eventConsumed = inputMap->OnEvent(event);
			}
		}

		//controller plug in/out
		switch (event.type)
		{
		case SDL_CONTROLLERDEVICEADDED:
			if (SDL_NumJoysticks() > 0)
			{
				InitController(event.cdevice.which);
			}
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
		{
			SDL_GameController* controller = SDL_GameControllerFromInstanceID(event.cdevice.which);
			SDL_GameControllerClose(controller);
		}
		break;
		default:
			break;
		}

		return eventConsumed;
	}

	void InputSystem::StartRebind(sptr<Input> input, int peripherals, int version)
	{
		m_forbiddenKeys.clear();
		m_forbiddenButtons.clear();
		m_forbiddenTriggers.clear();
		m_forbiddenMouses.clear();

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
		m_forbiddenKeys.clear();
		m_forbiddenButtons.clear();
		m_forbiddenTriggers.clear();
		m_forbiddenMouses.clear();

		m_rebind = nullptr;
		m_rebindVersion = 0;
		m_rebindPeripherals = 0;
	}
	void InputSystem::AddForbiddenBinding(KeyScancode key)
	{
		m_forbiddenKeys.push_back(key);
	}
	void InputSystem::AddForbiddenBinding(ControllerButton button)
	{
		m_forbiddenButtons.push_back(button);
	}
	void InputSystem::AddForbiddenBinding(ControllerTrigger trigger)
	{
		m_forbiddenTriggers.push_back(trigger);
	}
	void InputSystem::AddForbiddenBinding(MouseButton mouse)
	{
		m_forbiddenMouses.push_back(mouse);
	}

	int InputSystem::GetUsedMethod()
	{
		return System::Method::Event;
	}

	bool InputSystem::IsRebinding()
	{
		return !(m_rebind == nullptr);
	}
	sptr<Input> InputSystem::GetRebindingInput()
	{
		return m_rebind;
	}
	bool InputSystem::Rebind(SDL_Event& e)
	{
		switch (m_rebind->GetType())
		{
		case InputType::Button:
		{
			auto buttonInput = static_pointer_cast<ButtonInput>(m_rebind);
			int version = m_rebindVersion;
			if (m_rebindVersion >= buttonInput->GetBindingsCount())
			{
				version = -1;
			}

			if ((m_rebindPeripherals & PeripheralFlag::Mouse) == PeripheralFlag::Mouse)
			{
				//Mouse
				switch (e.type)
				{
				case SDL_MOUSEBUTTONDOWN:
					if (Container::Contains(m_forbiddenMouses, (MouseButton)e.button.button))
						return false;

					if (version == -1)
					{
						buttonInput->AddMouse((MouseButton)e.button.button);
						EndRebind();
						return true;
					}
					else
					{
						buttonInput->SetMouse(version, (MouseButton)e.button.button);
						EndRebind();
						return true;
					}
					break;
				}
			}

			if ((m_rebindPeripherals & PeripheralFlag::Keyboard) == PeripheralFlag::Keyboard)
			{
				//Keyboard
				switch (e.type)
				{
				case SDL_KEYDOWN:
					if(Container::Contains(m_forbiddenKeys, (KeyScancode)e.key.keysym.scancode))
						return false;

					if (version == -1)
					{
						buttonInput->AddKey((KeyScancode)e.key.keysym.scancode);
						EndRebind();
						return true;
					}
					else
					{
						buttonInput->SetKey(version, (KeyScancode)e.key.keysym.scancode);
						EndRebind();
						return true;
					}
					break;
				}
			}

			if ((m_rebindPeripherals & PeripheralFlag::Controller) == PeripheralFlag::Controller)
			{
				//Controller
				switch (e.type)
				{
				case SDL_CONTROLLERBUTTONDOWN:
					if (Container::Contains(m_forbiddenButtons, (ControllerButton)e.cbutton.button))
						return false;

					if (version == -1)
					{
						buttonInput->AddControllerButton((ControllerButton)e.cbutton.button);
						EndRebind();
						return true;
					}
					else
					{
						buttonInput->SetControllerButton(version, (ControllerButton)e.cbutton.button);
						EndRebind();
						return true;
					}
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
					if (Container::Contains(m_forbiddenTriggers, ControllerTrigger::Left))
						return false;

					if (version == -1)
					{
						buttonInput->AddControllerTrigger(ControllerTrigger::Left);
						EndRebind();
						return true;
					}
					else
					{
						buttonInput->SetControllerTrigger(version, ControllerTrigger::Left);
						EndRebind();
						return true;
					}
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
					if (Container::Contains(m_forbiddenTriggers, ControllerTrigger::Right))
						return false;

					if (version == -1)
					{
						buttonInput->AddControllerTrigger(ControllerTrigger::Right);
						EndRebind();
						return true;
					}
					else
					{
						buttonInput->SetControllerTrigger(version, ControllerTrigger::Right);
						EndRebind();
						return true;
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
		return false;
	}
	void InputSystem::InitController(int id)
	{
		SDL_GameController* controller = SDL_GameControllerOpen(id);
		if (controller == NULL)
		{
			LOG_INFO("Could not open game controller: {0}\n", SDL_GetError());
		}
	}
}