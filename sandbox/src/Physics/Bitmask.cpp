#include "pch.h"
#include "Sandbox/Physics/Bitmask.h"
#include "Sandbox/Core/Log.h"

namespace Sandbox
{
	Filter::Filter() : m_maxFlags(16)
	{

	}
	void Filter::AddFlag(String name)
	{
		if (m_flags.size() >= m_maxFlags)
		{
			LOG_WARN("Max flag count reached for filter when trying to add " + name);
		}
		else
		{
			uint16_t flag = (uint16_t)std::pow(2, m_flags.size());
			if (!m_flags.insert(MakePair(name, flag)).second)
			{
				LOG_WARN("Filter flag already exists: " + name);
			}
		}
	}

	bool Filter::BitmaskContains(Bitmask mask, String flag)
	{
		auto find_it = m_flags.find(flag);
		if (find_it == m_flags.end())
		{
			LOG_WARN("Filter's flag doesn't exists: " + flag + ", false returned.");
			return false;
		}
		return mask.Contains(find_it->second);
	}
}