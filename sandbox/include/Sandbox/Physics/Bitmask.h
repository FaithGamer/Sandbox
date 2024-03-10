#pragma once

#include <cstdint>
#include <unordered_map>
#include "Sandbox/Core/std_macros.h"
#include "Sandbox/Core/Log.h"

namespace Sandbox
{
	/// @brief 16 bit bitmask allowing for 31 flags
	class Bitmask
	{
	public:
		Bitmask() : flags(0)
		{

		}
		Bitmask(uint16_t Flags) : flags(Flags)
		{

		}
		void Clear()
		{
			flags = 0;
		}
		void AddFlag(uint16_t flag)
		{
			flags = flags | flag;
		}
		bool Contains(uint16_t flag)
		{
			return (flags & flag) == flag;
		}
		uint16_t flags;
	};


	/// @brief Dynamic collection of named flags, enabling the generation of bitmasks with custom names for each flag
	class Filter 
	{
	public:
		Filter();
		void AddFlag(String name);

		/// @brief Generate a bitmask for the given flags names. 
		/// Flags must have been created first with the AddFlag method.
		/// @tparam ...str bewteen 1 and 31 String corresponding to the flags
		template<typename... Str>
		Bitmask GetMask(Str ...filters)
		{
			Bitmask mask;
			SetMask(mask, filters...);
			return mask;
		}
		inline std::unordered_map<String, uint16_t> GetFlags()
		{
			return m_flags;
		}

		bool BitmaskContains(Bitmask mask, String flag);

	private:
		template<typename... str>
		void SetMask(Bitmask& mask, String filter, str ...filters)
		{
			auto find_it = m_flags.find(filter);
			if (find_it == m_flags.end())
			{
				LOG_ERROR("Filter's flag doesn't exists: " + filter + ", couldn't generate a proper bitmask.");
			}
			else
			{
				uint16_t flag = find_it->second;
				mask.AddFlag(flag);
				SetMask(mask, filters...);
			}
		}
		void SetMask(Bitmask& mask)
		{

		}

		Bitmask m_allFlags;
		size_t m_maxFlags;
		std::unordered_map<String, uint16_t> m_flags;
	};


}
