#pragma once


///@brief Bind any Type with a unique identifier.
/// id is determined the first time GetId is called

namespace Sandbox
{
	class TypeId
	{
	public:
		template <typename T>
		static int32_t GetId()
		{
			static const int32_t id = currentTypeId++;
			return id;
		}
		static int32_t currentTypeId;
	};
}