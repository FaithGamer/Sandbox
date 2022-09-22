#include "pch.h"
#include "Vec.h"
#include <glm/glm.hpp>
#include "Camera.h"


#include <glm/gtc/matrix_transform.hpp>

namespace sandbox
{
	Camera::Camera() 
		:m_position(0, 1, -1), m_target(0, 0, 0), m_upDirection(0, 1, 0), 
		m_fieldOfView(45.f), m_aspectRatio(1), m_nearClippingPlane(0.1f), m_farClippingPlane(100.f), 
		m_needComputeProjectionMatrix(true), m_needComputeViewMatrix(true), m_projectionMatrix(1.f), m_viewMatrix(1.f)
	{
	}

	void Camera::SetPosition(Vec3f position)
	{
		m_position = position;
		m_needComputeViewMatrix = true;
	}

	void Camera::SetTarget(Vec3f target)
	{
		m_target = target;
		m_needComputeViewMatrix = true;
	}

	void Camera::SetFieldOfView(float fieldOfView)
	{
		m_fieldOfView = fieldOfView;
		m_needComputeProjectionMatrix = true;
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
		m_needComputeProjectionMatrix = true;
	}

	Mat4 Camera::GetViewMatrix() const
	{
		if (m_needComputeViewMatrix)
		{
			ComputeViewMatrix();
			m_needComputeViewMatrix = false;
		}
		return m_viewMatrix;
	}

	Mat4 Camera::GetProjectionMatrix() const
	{
		if (m_needComputeProjectionMatrix)
		{
			ComputeProjectionMatrix();
			m_needComputeProjectionMatrix = false;
		}
		return m_projectionMatrix;
	}

	void Camera::ComputeViewMatrix() const
	{
		m_viewMatrix = glm::lookAt(m_position, m_target, m_upDirection);
	}

	void Camera::ComputeProjectionMatrix() const
	{
		m_projectionMatrix = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearClippingPlane, m_farClippingPlane);
	}
}