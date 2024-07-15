﻿#pragma once

#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/Keyboard.h"
#include "Sandbox/Input/Controller.h"
#include "Sandbox/Input/Mouse.h"
#include "Sandbox/Input/Bindings.h"
#include "Sandbox/Core/Vec.h"


namespace Sandbox
{
	//To do: add support for mouse wheel

	/// @brief What state the input is currently in.
	/// This is also the data sent by the broadcasted signal when the input is triggered.
	class ButtonInputState : public InputSignal
	{

	public:
		bool GetBool() const override { return pressed; }
		float GetFloat() const override { return pressedAmount; }
		bool pressed = false;
		float pressedAmount = 0.0f;
	};

	class ButtonInput : public Input
	{
	public:
		ButtonInput(std::string name);

		/// @brief Set all the versions of the keys, controller buttons, controller triggers, mouse buttons, that are bound to this input.
		/// @param bindings 
		virtual void SetBindings(const ButtonBindings& bindings);

		virtual std::string GetName() const override;
		virtual InputType GetType() const override;

	public:

		/// @brief Bind a Key from the keyboard
		/// @param keyButton The scancode of the key
		void AddKey(KeyScancode keyButton);
		/// @brief Add a mouse button 
		/// @param mouseButton The mouse button
		void AddMouse(MouseButton mouseButton);
		/// @brief Add to a controller button
		/// @param controllerButton the controller button
		void AddControllerButton(ControllerButton controllerButton);
		/// @brief Add to a controller trigger.
		/// It will send signal every time the pressing amount changes.
		/// @param trigger The controller trigger
		void AddControllerTrigger(ControllerTrigger trigger);
		/// @brief Set a specific binding key
		/// @param version Wich binding version, you can get the count of versions with GetBindingsCount
		/// @param keyButton The key to bind to
		void SetKey(int version, KeyScancode keyButton);
		/// @brief Set a mouse button to a specific binding 
		/// @param version Wich binding version, you can get the count of versions with GetBindingsCount
		/// @param keyButton The key to bind to
		void SetMouse(int version, MouseButton mouseButton);
		/// @brief Set a controller button to a specific binding 
		/// @param version Wich binding version, you can get the count of versions with GetBindingsCount
		/// @param mouseButton the mouse button to bind to
		void SetControllerButton(int version, ControllerButton controllerButton);
		/// @brief Set a controller trigger to a specific binding 
		/// @param version Wich binding version, you can get the count of versions with GetBindingsCount
		/// @param controllerButton the controller button to binds to
		void SetControllerTrigger(int version, ControllerTrigger controllerTrigger);
		/// @brief Remove a binding
		void RemoveBinding(int version);

		/// @brief Set wether or not the signal is broadcasted when the button is pressed
		/// By default yes
		/// @param signalOnPress true = yes, false = no
		void SetSendSignalOnPress(bool signalOnPress);
		/// @brief Set wether or not the signal is broadcasted when the button is released
		/// By default no
		/// @param signalOnRelease true = yes, false = no
		void SetSendSignalOnRelease(bool signalOnRelease);
		/// @brief Set how much a trigger must be pressed to switch the pressed state
		/// @param deadzone ranging from 0.0 to 1.0. 1.0 = 100% deadzone
		/// @param controllerTrigger the controller trigger to binds to
		void SetTriggerDeadzone(float sensitivity);

		/// @brief Get the count of different bindings
		int GetBindingsCount() const;
		/// @brief Get the bindings
		ButtonBindings& GetBindings();

		bool HaveBinding(KeyScancode key);
		bool HaveBinding(MouseButton mouse);
		bool HaveBinding(ControllerButton button);
		bool HaveBinding(ControllerTrigger trigger);


	protected:

		bool KeyPressed(const SDL_Event& e) override;
		bool KeyReleased(const SDL_Event& e) override;
		bool MouseButtonPressed(const SDL_Event& e) override;
		bool MouseButtonReleased(const SDL_Event& e) override;
		bool ControllerButtonPressed(const SDL_Event& e) override;
		bool ControllerButtonReleased(const SDL_Event& e) override;
		bool ControllerTriggerMoved(const SDL_Event& e) override;
		void UpdateEventListened() override;

		bool ReleaseButton();
		bool PressButton();
		bool SetPressedAmount(float amount);

	private:

		ButtonBindings m_bindings;

		bool m_sendSignalOnPress;
		bool m_sendSignalOnRelease;
		float m_triggerDeadzone;

		double m_lastTriggerValue;

		ButtonInputState m_state;

		std::string m_name;
	};
}