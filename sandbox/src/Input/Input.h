#pragma once

#include "Trigger.h"

namespace sandbox
{
	/// <summary>
	/// Holds one player input, the underlaying trigger determine how it is getting actionned.
	/// Keep track of the trigger state and dispatch events to listeners
	/// Can bind to multiple triggers of the same type
	/// </summary>
	class Input
	{
	public:
		Input(uptr<Trigger> trigger);

		void UpdateState(SDL_Event& e);
		void ListenAndSetTrigger(int version);
		void PushTrigger(uptr<Trigger> trigger);
		void SetTrigger(uptr<Trigger> trigger, int version);

		void BindKeyButton(SDL_Scancode keyButton, int directional = -1);
		void BindMouseButton(Uint8 mouseButton, int directional = -1);
		void BindControllerButton(SDL_GameControllerButton controllerButton, int directional = -1);
		void BindControllerStick(ControllerStick stick);

		int GetTriggersCount() const;
		bool IsActivated() const;
		uptr<Trigger> GetTrigger(int version = 0);
		std::string GetName() const;
		TriggerType GetTriggerType() const;


	private:
		std::vector<uptr<Trigger>> m_triggers;
		TriggerType m_triggerType;
		std::string m_name;
	};
}