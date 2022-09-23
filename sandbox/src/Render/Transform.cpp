#include "pch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace sandbox
{
	Transform::Transform() :
		m_translation(0.f, 0.f, 0.f), m_scale(1.f, 1.f, 1.f), m_rotation(0.f), m_origin(0.f, 0.f, 0.f), m_transformMatrix(1.f)
	{
	}

	Transform::Transform(Vec3f translation, Vec3f scale, float angle, Vec3f origin)
		:m_translation(translation), m_scale(scale), m_rotation(angle), m_origin(origin), m_transformMatrix(1.f)
	{
		m_needCompute = true;
	}

	void Transform::SetTranslation(Vec3f translation)
	{
		m_translation = translation;
		m_needCompute = true;
	}

	void Transform::SetScale(Vec3f scale)
	{
		m_scale = scale;
		m_needCompute = true;
	}

	void Transform::SetRotation(float angle)
	{
		m_rotation = angle;
		m_needCompute = true;
	}

	void Transform::SetOrigin(Vec3f origin)
	{
		m_origin = origin;
		m_needCompute = true;
	}

	void Transform::Translate(Vec3f translation)
	{
		m_translation += translation;
		m_needCompute = true;
	}

	void Transform::Scale(Vec3f scale)
	{
		m_scale *= scale;
		m_needCompute = true;
	}

	void Transform::Rotate(float angle)
	{
		m_rotation += angle;
		m_needCompute = true;
	}

	void Transform::Reset()
	{
		m_rotation = 0.f;
		m_scale = Vec3f(1.f);
		m_translation = Vec3f(1.f);
		m_origin = Vec3f(1.f);

		m_needCompute = true;
	}

	Transform& Transform::operator+(const Transform& trans)
	{
		Transform t(m_translation + trans.m_translation,
			m_scale * trans.m_scale,
			m_rotation + trans.m_rotation,
			m_origin + trans.m_origin);

		return t;
	}

	Transform& Transform::operator+=(const Transform& trans)
	{
		m_translation += trans.m_translation;
		m_scale *= trans.m_scale;
		m_rotation += trans.m_rotation;
		m_origin += trans.m_origin;

		m_needCompute = true;

		return *this;
	}

	Transform& Transform::operator=(const Transform& trans)
	{
		m_translation = trans.m_translation;
		m_scale = trans.m_scale;
		m_rotation = trans.m_rotation;
		m_origin = trans.m_origin;

		m_needCompute = true;

		return *this;
	}

	Vec3f Transform::GetPosition() const
	{
		return Vec3f();
	}

	Vec3f Transform::GetScale() const
	{
		return Vec3f();
	}

	float Transform::GetRotation() const
	{
		return m_rotation;
	}

	Vec3f Transform::GetOrigin() const
	{
		return Vec3f();
	}

	Mat4 Transform::GetTransformMatrix() const
	{
		if (m_needCompute)
		{
			ComputeMatrix();
			m_needCompute = false;
		}
		return m_transformMatrix;
	}

	void Transform::ComputeMatrix() const
	{
		Mat4 transform(1.f);

		transform = glm::translate(transform, m_translation);
		transform = glm::rotate(transform, glm::radians(-m_rotation), glm::vec3(1, 0, 0));
		transform = glm::scale(transform, m_scale);
		transform = glm::translate(transform, -m_origin);

		m_transformMatrix = transform;
	}
}