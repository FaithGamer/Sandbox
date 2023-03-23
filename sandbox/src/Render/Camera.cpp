#include "pch.h"

#include "Vec.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/functions.hpp>

namespace Sandbox
{
	Camera::Camera()
		:m_position(0, 1, -1), m_target(0, 0, 0), m_worldUp(0, 1, 0), m_localBack(0, 0, 1), m_localRight(1, 0, 0), m_localUp(0, 1, 0),
		m_yaw(0), m_pitch(0), m_roll(0),
		m_fieldOfView(45.f), m_aspectRatio(1), m_nearClippingPlane(0.1f), m_farClippingPlane(100.f),
		m_needComputeProjectionMatrix(true), m_needComputeViewMatrix(true), m_projectionMatrix(1.f), m_viewMatrix(1.f)
	{
	}

	void Camera::SetPosition(Vec3f position)
	{
		m_position = position;
		m_needComputeViewMatrix = true;
	}

	void Camera::SetRotation(Vec3f eulerAngles)
	{
		m_yaw = eulerAngles.x;
		m_pitch = eulerAngles.y;
		m_roll = eulerAngles.z;
		ComputeDirection();
		m_needComputeViewMatrix = true;
	}

	void Camera::SetTarget(Vec3f target)
	{
		m_target = target;
		m_needComputeViewMatrix = true;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		m_position = Vec3f(x, y, z);
		m_needComputeViewMatrix = true;
	}

	void Camera::SetRotation(float x, float y, float z)
	{
		m_yaw = x;
		m_pitch = y;
		m_roll = z;
		ComputeDirection();
		m_needComputeViewMatrix = true;
	}

	void Camera::SetTarget(float x, float y, float z)
	{
		m_target = Vec3f(x, y, z);
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

	void Camera::MoveWorld(Vec3f offset)
	{
		m_position += offset;
		m_needComputeViewMatrix = true;
	}

	void Camera::MoveWorld(float x, float y, float z)
	{
		m_position += Vec3f(x, y, z);
		m_needComputeProjectionMatrix = true;
	}

	void Camera::MoveLocalX(float offset)
	{
		m_position += Vec3f(m_localRight.x * offset,
			m_localRight.y * offset,
			m_localRight.z * offset);
		m_needComputeViewMatrix = true;
	}

	void Camera::MoveLocalZ(float offset)
	{
		m_position += Vec3f(m_localBack.x * offset,
			m_localBack.y * offset,
			m_localBack.z * offset);
		m_needComputeViewMatrix = true;
	}
	void Camera::Yaw(float yaw)
	{
		m_yaw += yaw;
		ComputeDirection();
		m_needComputeViewMatrix = true;
	}

	void Camera::Pitch(float pitch)
	{
		m_pitch += pitch;
		ComputeDirection();
		m_needComputeViewMatrix = true;

	}

	void Camera::Roll(float roll)
	{
		m_roll += roll;
		ComputeDirection();
		m_needComputeViewMatrix = true;
	}

	void Camera::SetYaw(float yaw)
	{
		m_yaw = yaw;
		ComputeDirection();
		m_needComputeViewMatrix = true;
	}

	void Camera::SetPitch(float pitch)
	{
		m_pitch = pitch;
		ComputeDirection();
		m_needComputeViewMatrix = true;
	}

	void Camera::SetRoll(float roll)
	{
		m_roll = roll;
		ComputeDirection();
		m_needComputeViewMatrix = true;
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

	Mat4 Camera::GetTargetViewMatrix() const
	{
		return glm::lookAt(m_position, m_target, m_worldUp);
	}

	void Camera::ComputeViewMatrix() const
	{

		m_viewMatrix = glm::lookAt(m_position, m_position + m_localBack, m_localUp);

		//view matrix will be composed like this
		// 
		//		r = localRight
		//		u = localUp
		//		d = localBack
		//		p = position

		/*

		r.x, r.y, r.z, p.x
		u.x, u.y, u.z, p.y
		d.x, d.y, d.z, p.z
		0.0, 0.0, 0.0, 1.0

		*/
	}

	void Camera::ComputeDirection()
	{
		//Calculate view direction (in fact the opposite this is why it's called back here)
		glm::vec3 back;
		back.x = cos(glm::radians(m_yaw-90.0f)) * cos(glm::radians(m_pitch));
		back.y = sin(glm::radians(m_pitch));
		back.z = sin(glm::radians(m_yaw-90.0f)) * cos(glm::radians(m_pitch));

		// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_localBack = glm::normalize(back);
		m_localRight = glm::normalize(glm::cross(m_localBack, m_worldUp));
		/*Vec3f rightWithRoll = m_localRight;
		rightWithRoll.x += cos(glm::radians(m_roll));
		rightWithRoll.y -= sin(glm::radians(m_roll));*/
		m_localUp = glm::normalize(glm::cross(m_localRight, m_localBack));
	}

	void Camera::ComputeProjectionMatrix() const
	{
		m_projectionMatrix = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearClippingPlane, m_farClippingPlane);
	}
}