#pragma once

#include "Sandbox/Vec.h"

namespace Sandbox
{
	class Camera
	{
	public:
		Camera();
		~Camera();
		void SetOrthographic(bool orthographic);
		/// @brief Set the zoom only in orthographic mode
		/// @param zoom 0.5 = zoom x2, 2 = zoom x0.5
		void SetOrthographicZoom(float zoom);
		void SetPosition(Vec3f position);
		void SetRotation(Vec3f eulerAngles);
		void SetTarget(Vec3f target);
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetTarget(float x, float y, float z);
		void SetFieldOfView(float fieldOfView);
		void SetAspectRatio(float aspectRatio);
		void SetAspectRatio(Vec2u xOverY);
		void SetNearClippingPlane(float nearClippingPlane);
		void SetFarClippingPlace(float farClippingPlane);

		void MoveWorld(Vec3f offset);
		void MoveWorld(float x, float y, float z);
		void MoveLocalX(float offset);
		void MoveLocalZ(float offset);
		void Yaw(float yaw);
		void Pitch(float pitch);
		void Roll(float roll);
		void SetYaw(float yaw);
		void SetPitch(float pitch);
		void SetRoll(float roll);
		
		Mat4 GetViewMatrix() const;
		Mat4 GetProjectionMatrix() const;
		Mat4 GetTargetViewMatrix() const;

	private:
		void ComputeViewMatrix() const;
		void ComputeProjectionMatrix() const;
		void ComputeDirection();

		Vec3f m_target;

		Vec3f m_position;
		Vec3f m_worldUp;
		Vec3f m_localBack;
		Vec3f m_localRight;
		Vec3f m_localUp;

		float m_yaw;
		float m_pitch;
		float m_roll;

		float m_fieldOfView;
		float m_aspectRatio;
		float m_nearClippingPlane;
		float m_farClippingPlane;
		float m_orthographicZoom;

		mutable Mat4 m_projectionMatrix;
		mutable Mat4 m_viewMatrix;
		mutable bool m_needComputeViewMatrix;
		mutable bool m_needComputeProjectionMatrix;

		bool m_orthographic;
	};

}