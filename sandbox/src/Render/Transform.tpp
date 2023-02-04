
namespace Sandbox
{
	constexpr Transform::Transform() :
		m_translation(0.f, 0.f, 0.f), m_scale(1.f, 1.f, 1.f), m_rotation(0.f), m_origin(0.f, 0.f, 0.f), m_transformMatrix(1.f), m_needCompute(true)
	{
	}

	constexpr Transform::Transform(Vec3f translation, Vec3f scale, float angle, Vec3f origin)
		: m_translation(translation), m_scale(scale), m_rotation(angle), m_origin(origin), m_transformMatrix(1.f), m_needCompute(true)
	{
	}

	constexpr void Transform::SetTranslation(Vec3f translation)
	{
		m_translation = translation;
		m_needCompute = true;
	}

	constexpr void Transform::SetScale(Vec3f scale)
	{
		m_scale = scale;
		m_needCompute = true;
	}

	constexpr void Transform::SetRotation(float angle)
	{
		m_rotation = angle;
		m_needCompute = true;
	}

	constexpr void Transform::SetOrigin(Vec3f origin)
	{
		m_origin = origin;
		m_needCompute = true;
	}

	constexpr void Transform::Translate(Vec3f translation)
	{
		m_translation += translation;
		m_needCompute = true;
	}

	constexpr void Transform::Scale(Vec3f scale)
	{
		m_scale *= scale;
		m_needCompute = true;
	}

	constexpr void Transform::Rotate(float angle)
	{
		m_rotation += angle;
		m_needCompute = true;
	}

	constexpr void Transform::Reset()
	{
		m_rotation = 0.f;
		m_scale = Vec3f(1.f);
		m_translation = Vec3f(1.f);
		m_origin = Vec3f(1.f);

		m_needCompute = true;
	}

	constexpr Transform Transform::operator+(const Transform& trans)
	{
		Transform t(m_translation + trans.m_translation,
			m_scale * trans.m_scale,
			m_rotation + trans.m_rotation,
			m_origin + trans.m_origin);
		//todo see warning
		return t;
	}

	constexpr Transform& Transform::operator+=(const Transform& trans)
	{
		m_translation += trans.m_translation;
		m_scale *= trans.m_scale;
		m_rotation += trans.m_rotation;
		m_origin += trans.m_origin;

		m_needCompute = true;

		return *this;
	}

	constexpr Transform& Transform::operator=(const Transform& trans)
	{
		m_translation = trans.m_translation;
		m_scale = trans.m_scale;
		m_rotation = trans.m_rotation;
		m_origin = trans.m_origin;

		m_needCompute = true;

		return *this;
	}

	constexpr Vec3f Transform::GetTranslation() const
	{
		return m_translation;
	}

	constexpr Vec3f Transform::GetScale() const
	{
		return m_scale;
	}

	constexpr float Transform::GetRotation() const
	{
		return m_rotation;
	}

	constexpr Vec3f Transform::GetOrigin() const
	{
		return m_origin;
	}

	constexpr Mat4 Transform::GetTransformMatrix() const
	{
		if (m_needCompute)
		{
			ComputeMatrix();
			m_needCompute = false;
		}
		return m_transformMatrix;
	}
}