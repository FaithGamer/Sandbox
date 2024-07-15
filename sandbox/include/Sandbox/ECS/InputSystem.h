#pragma once

#include "Sandbox/ECS/System.h"
#include "Sandbox/Input/Input.h"

namespace Sandbox
{
	class InputSystem : public System
	{
	public:
		enum PeripheralFlag : int
		{
			Mouse = 1,
			Keyboard = 2,
			Controller = 4
		};
	public:
		InputSystem();
		bool OnEvent(SDL_Event& event);
		/// @brief Every subsequent keyboard/mouse/controller event will try to bind to this input
		/// Call EndRebind()  when you're done.
		/// Warning: at the moment you can rebind only Button Input
		/// @param peripheral set flags of type InputSystem::PeripheralFlag
		/// @param version pass -1 to add a new version
		void StartRebind(sptr<Input> input, int peripherals, int version = 0);
		//To do, need a rework of the directional input to be able to select which direction we wanna change button 
		///void SetRebindDirection(Vec2f direction);
		void EndRebind();
		int GetUsedMethod();
		bool IsRebinding();
	private:
		void Rebind(SDL_Event& e);
	private:
		//Rebinding
		sptr<Input> m_rebind;
		int m_rebindPeripherals = 0;
		int m_rebindVersion = -1;

	};
}
