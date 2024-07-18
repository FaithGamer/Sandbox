#pragma once

#include "Sandbox/ECS/System.h"
#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/Controller.h"
#include "Sandbox/Input/Mouse.h"
#include "Sandbox/Input/Keyboard.h"
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
		void OnStart() override;
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

		/// @brief Add a button that will be ignored during the rebinding, this will reset after a call to EndRebind
		void AddForbiddenBinding(KeyScancode key);
		/// @brief Add a button that will be ignored during the rebinding, this will reset after a call to EndRebind
		void AddForbiddenBinding(ControllerButton button);
		/// @brief Add a button that will be ignored during the rebinding, this will reset after a call to EndRebind
		void AddForbiddenBinding(ControllerTrigger trigger);
		/// @brief Add a button that will be ignored during the rebinding, this will reset after a call to EndRebind
		void AddForbiddenBinding(MouseButton mouse);
		int GetUsedMethod();
		bool IsRebinding();
		sptr<Input> GetRebindingInput();
	private:
		bool Rebind(SDL_Event& e);
		void InitController(int id);
	private:
		//Rebinding
		sptr<Input> m_rebind;
		int m_rebindPeripherals = 0;
		int m_rebindVersion = -1;
		std::vector<KeyScancode> m_forbiddenKeys;
		std::vector<ControllerButton> m_forbiddenButtons;
		std::vector<ControllerTrigger> m_forbiddenTriggers;
		std::vector<MouseButton> m_forbiddenMouses;

	};
}
