#pragma once

namespace sandbox
{
	class Camera
	{
	public:
		Camera();

		void SetPosition(Vec3f position);
		void SetRotation(Vec3f eulerAngles);
		void SetTarget(Vec3f target);
		void SetFieldOfView(float fieldOfView);
		void SetAspectRatio(float aspectRatio);

		void MoveWorld(Vec3f offset);
		void MoveLocalX(float offset);
		void MoveLocalZ(float offset);
		void Yaw(float yaw);
		void Pitch(float pitch);
		void Roll(float roll);
		
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

		mutable Mat4 m_projectionMatrix;
		mutable Mat4 m_viewMatrix;
		mutable bool m_needComputeViewMatrix;
		mutable bool m_needComputeProjectionMatrix;
	};

}