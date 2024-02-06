#include "pch.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox/Render/Transform.h"

#include "Sandbox/ECS/Entity.h"
namespace Sandbox
{
	Transform::Transform() :
		m_translation(0.f),
		m_scale(1.f),
		m_rotation(0.f),
		m_transformMatrix(1.f),
		needCompute(true),
		matrixUpdated(true),
		m_haveParent(false),
		m_parent(EntityId(0))

	{
	}

	Transform::Transform(Vec3f translation, Vec3f scale, float angle)
		: m_translation(translation),
		m_scale(scale),
		m_rotation(0.f, 0.f, angle),
		m_transformMatrix(1.f),
		needCompute(true),
		matrixUpdated(true),
		m_parent(EntityId(0))

	{
	}

	Transform::Transform(Vec3f translation, Vec3f scale, Vec3f angles)
		: m_translation(translation),
		m_scale(scale),
		m_rotation(angles),
		m_transformMatrix(1.f),
		needCompute(true),
		matrixUpdated(true),
		m_haveParent(false),
		m_parent(EntityId(0))

	{
	}

	void Transform::SetParent(EntityId entity)
	{
		m_parent = entity;
		m_haveParent = true;
	}

	void Transform::RemoveParent()
	{
		m_haveParent = false;
	}

	void Transform::SetPosition(Vec3f translation)
	{
		if (translation == m_translation)
			return;
		m_translation = translation;
		needCompute = true;
	}

	void Transform::SetScale(Vec3f scale)
	{
		if (m_scale == scale)
			return;
		m_scale = scale;
		needCompute = true;
	}

	/*void Transform::SetOrigin(Vec3f origin)
	{
		if (m_origin == origin)
			return;
		m_origin = origin;
		needCompute = true;
	}*/

	void Transform::SetPosition(float x, float y, float z)
	{
		auto pos = Vec3f(x, y, z);
		if (m_translation == pos)
			return;
		m_translation = pos;
		needCompute = true;
	}

	void Transform::SetScale(float x, float y, float z)
	{
		auto scale = Vec3f(x, y, z);
		if (m_scale == scale)
			return;
		m_scale = Vec3f(x, y, z);
		needCompute = true;
	}

	/*void Transform::SetOrigin(float x, float y, float z)
	{
		auto origin = Vec3f(x, y, z);
		if (m_origin == origin)
			return;
		m_origin = Vec3f(x, y, z);
		needCompute = true;
	}*/

	void Transform::SetRotation(Vec3f angles)
	{
		if (m_rotation == angles)
			return;
		m_rotation = angles;
		needCompute = true;
	}

	void Transform::SetRotationZAxis(float angle)
	{
		if (m_rotation.z == angle)
			return;
		m_rotation.z = angle;
		needCompute = true;
	}

	void Transform::Move(Vec3f translation)
	{
		m_translation += translation;
		needCompute = true;
	}

	void Transform::Move(float x, float y, float z)
	{
		auto prev = m_translation;
		m_translation += Vec3f(x, y, z);
		needCompute = true;
	}

	void Transform::Scale(Vec3f scale)
	{
		auto prev = m_scale;
		m_scale *= scale;
		needCompute = true;
	}

	void Transform::Scale(float x, float y, float z)
	{
		m_scale *= Vec3f(x, y, z);
		needCompute = true;
	}

	void Transform::Rotate(Vec3f angles)
	{
		//TO DO: Quaternions
		m_rotation += angles;
		needCompute = true;
	}

	void Transform::RotateZAxis(float angle)
	{
		//TO DO: Quaternions
		m_rotation.z += angle;
		needCompute = true;
	}

	void Transform::Reset()
	{
		m_rotation = Vec3f(0.f);
		m_scale = Vec3f(1.f);
		m_translation = Vec3f(0.f);
		m_origin = Vec3f(0.f);

		needCompute = true;
	}

	Transform Transform::operator+(const Transform& trans)
	{
		Transform t(m_translation + trans.m_translation,
			m_scale * trans.m_scale,
			m_rotation + trans.m_rotation);
		//todo see warning
		return t;
	}

	Transform& Transform::operator+=(const Transform& trans)
	{
		m_translation += trans.m_translation;
		m_scale *= trans.m_scale;
		m_rotation += trans.m_rotation;
		m_origin += trans.m_origin;

		needCompute = true;

		return *this;
	}

	Transform& Transform::operator=(const Transform& trans)
	{
		m_translation = trans.m_translation;
		m_scale = trans.m_scale;
		m_rotation = trans.m_rotation;
		m_origin = trans.m_origin;

		needCompute = true;

		return *this;
	}

	Vec3f Transform::GetPosition() const
	{
		return m_translation;
	}

	Vec3f Transform::GetWorldPosition() const
	{
		if (m_haveParent)
		{
			return m_translation + Entity(m_parent).GetComponent<Transform>()->GetWorldPosition();
		}
		return m_translation;
	}

	Vec3f Transform::GetScale() const
	{
		return m_scale;
	}

	Vec3f Transform::GetWorldScale() const
	{
		if (m_haveParent)
		{
			return m_scale * Entity(m_parent).GetComponent<Transform>()->GetWorldScale();
		}
		return m_scale;
	}

	Vec3f Transform::GetRotation() const
	{
		return m_rotation;
	}

	Vec3f Transform::GetWorldRotation() const
	{
		if (m_haveParent)
		{
			return m_rotation + Entity(m_parent).GetComponent<Transform>()->GetWorldScale();
		}
		return m_rotation;
	}

	float Transform::GetRotationZAxis() const
	{
		return m_rotation.z;
	}

	Mat4 Transform::GetTransformMatrix() const
	{
		if (needCompute)
		{
			ComputeMatrix();
			needCompute = false;
		}
		if (m_haveParent)
		{
			auto parent = Entity(m_parent).GetComponent<Transform>();
			return parent->GetTransformMatrix() * m_transformMatrix;
		}
		return m_transformMatrix;
	}

	void Transform::ComputeMatrix() const
	{
		Mat4 transform(1.f);

		transform = glm::translate(transform, (glm::vec3)m_translation);
		transform = glm::rotate(transform, glm::radians(-m_rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, (glm::vec3)m_scale);

		m_transformMatrix = transform;
		matrixUpdated = true;
	}
}