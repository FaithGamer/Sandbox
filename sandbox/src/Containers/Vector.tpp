#include "Vector.h"
#include "Core/Log.h"

namespace Sandbox
{
	namespace Vector
	{
		template <typename T>
		int Remove(T toRemove, std::vector<T>& container)
		{
			int removedCount = 0;
			size_t i = 0;
			std::vector<size_t> is;
			for (auto& element : container)
			{
				if (element == toRemove)
				{
					is.emplace_back(i);
				}
				i++;
			}

			for (size_t j : is)
			{
				if (j >= container.size())
				{
					break;
				}
				//find the first value that differs from the one removed.
				//pop_back as long has a different value is not found.
				for (size_t r = container.size() - 1; r >= 0; r--)
				{
					if (j >= container.size())
					{
						break;
					}
					if (container.at(r) != container.at(j))
					{
						container.at(j) = container.at(r);
						container.pop_back();
						removedCount++;
						break;
					}

					container.pop_back();
					removedCount++;
				}
			}
			return removedCount;
		}

		template <typename T>
		bool PushIfDoesntContains(const T& value, std::vector<T>& container)
		{
			auto container_it = std::find(container.begin(), container.end(), value);
			if (container_it == container.end())
			{
				container.push_back(value);
			}
		}

		template <typename T>
		bool Contains(const T& value, std::vector<T>& container)
		{
			auto container_it = std::find(container.begin(), container.end(), value);
			if (container_it != container.end())
			{
				return true;
			}
			return false;
		}
	}
}