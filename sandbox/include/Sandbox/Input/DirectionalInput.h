#pragma once
#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Vec.h"

namespace Sandbox
{
	/// @brief What state the input is currently in.
	/// This is also the data sent by the broadcasted signal when the input is triggered.
	/// the final state is the sum of all the pressed buttons
	/// and sticks currentDirection vectors. The resulting vector's axis (x, y) are then clamped from 0 ot 1.
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
		DirectionalInput(std::string name);

		virtual void ListenEventAndBind(const SDL_Event& e, int version = 0) override;
		/// @brief Set all the versions of the various keys, controller buttons, controller triggers, mouse buttons
		/// and their corresponding directions, as well as the controller analog sticks, that are bound to this input.
		/// @param bindings 
		virtual void SetBindings(const DirectionalBindings& bindings);

		virtual std::string GetName() const override;
		InputType GetType() const override;

		/// @brief Add binding to a stick
		/// @param stick The controller stick
		void AddStick(ControllerStick stick);
		/// @brief Add binding to buttons, for example W, A, S, D
		/// @param buttons Each value of the vector contains a button bound with a direction
		/// When multiple buttons are being pressed at the same time, the final state is the sum of all the pressed buttons
	    /// and sticks direction vectors, where each axis (x, y) is clamped from 0 ot 1.
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

		/// @brief Check if the stick is used in any of the bindings
		/// @return true if yes
		bool HaveBinding(ControllerStick stick);
		/// @brief Check if any of the directional button in the vector matches any of the directional button in another binding
		/// @return true if yes
		bool HaveBinding(std::vector<DirectionalButton> buttons);
		/// @brief Check if the button is used in any of the bindings
		/// @return true if yes
		bool HaveBinding(Button button);
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
		void AxisMove(float value, bool x, Direction& direction);

		void ComputeState();

	private:
		std::string m_name;
		DirectionalBindings m_bindings;
		DirectionalInputState m_state;
		float m_triggerDeadzone;
		float m_stickDeadzone;

	};
}