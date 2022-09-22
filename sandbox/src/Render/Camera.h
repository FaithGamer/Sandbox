#pragma once

namespace sandbox
{
	class Camera
	{
	public:
		Camera();

		void SetPosition(Vec3f position);
		void SetTarget(Vec3f target);
		void SetFieldOfView(float fieldOfView);
		void SetAspectRatio(float aspectRatio);

		Mat4 GetViewMatrix() const;
		Mat4 GetProjectionMatrix() const;

	private:
		void ComputeViewMatrix() const;
		void ComputeProjectionMatrix() const;

		Vec3f m_target;
		Vec3f m_position;
		Vec3f m_upDirection;

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