#include "pch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace sandbox
{
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