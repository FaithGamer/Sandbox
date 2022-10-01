#pragma once
#include <memory>

/// <summary>
/// Bind any Type with a unique identifier
/// id is determined the first time GetId is called
/// </summary>
class TypeId
{
public:
	template <typename T>
	static int GetId()
	{
		static const int32_t id = currentTypeId++;
		return id;
	}
	static int32_t currentTypeId;
};


