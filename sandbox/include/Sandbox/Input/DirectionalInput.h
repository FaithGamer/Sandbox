#pragma once
#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Vec.h"

namespace Sandbox
{
	/// @brief What state the input is currently in.
	/// This is also the data sent by the broadcasted signal when the input is triggered.
	class DirectionalInputState : public InputSignal
	{
	public:
		Vec2f GetVec2f() const override { return direction; }
		Vec2f direction = Vec2f(0, 0);
	};

	//Using either a set of buttons or a stick to represent a Vec2f state
	class DirectionalInput : public Input
	{

	public:
		virtual void ListenEventAndBind(const SDL_Event& e, int version = 0) override;
		virtual void SetBindings(const DirectionalBindings& bindings);

		virtual std::string GetName() const override;
		InputType GetType() const override;

		/// @brief Add binding to a stick
		/// @param stick The controller stick
		void AddStick(ControllerStick stick);
		/// @brief Add binding to buttons, for example wqsd 
		/// @param buttons Each value of the vector contains a button bound with a direction
		/// When multiple buttons are being pressed at the same time, the final state is the sum of
		/// all the pressed button's direction.
		void AddButtons(std::vector<DirectionalButton> buttons);
		/// @brief Set a specific binding's stick
		/// @param version Binding version
		void SetStick(ControllerStick stick, int version);
		/// @brief Set a specific binding's buttons
		/// @param version Binding version
		void SetButtons(std::vector<DirectionalButton> buttons, int version);
		/// @brief Remove a binding
		/// @param version Binding version
		void RemoveBinding(int version);
		/// @brief Set stick deadzone
		/// @param deadzone range from 0.0 to 1.0, where 1.0 = 100% deadzone
		void SetStickDeadZone(float deadzone);
		/// @brief Set trigger deadzone
		/// @param deadzone range from 0.0 to 1.0, where 1.0 = 100% deadzone
		void SetTriggerDeadZone(float deadzone);

		bool HaveBinding(ControllerStick stick);
		/// @brief If any of the directional button in the vector matches any of the directional button in another binding, return true
		/// @param buttons 
		/// @return 
		bool HaveBinding(std::vector<DirectionalButton> buttons);
		/// @brief Get the count of bindings
		int GetBindingsCount() const;
		/// @brief Get the bindings
		DirectionalBindings& GetBindings();

	protected:
		virtual bool KeyPressed(const SDL_Event& e) override;
		virtual bool KeyReleased(const SDL_Event& e) override;
		virtual bool MouseButtonPressed(const SDL_Event& e) override;
		virtual bool MouseButtonReleased(const SDL_Event& e) override;
		virtual bool ControllerButtonPressed(const SDL_Event& e) override;
		virtual bool ControllerButtonReleased(const SDL_Event& e) override;
		virtual bool ControllerTriggerMoved(const SDL_Event& e) override;
		virtual bool ControllerStickMoved(const SDL_Event& e) override;

		virtual void UpdateEventListened() override;

	private:
		friend InputMap;

		DirectionalInput(std::string name);
		void ComputeState();

	private:
		std::string m_name;
		DirectionalBindings m_bindings;
		DirectionalInputState m_state;
		float m_triggerDeadzone;
		float m_stickDeadzone;

	};
}