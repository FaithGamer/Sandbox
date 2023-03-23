#pragma once
#include <vector>

namespace Sandbox
{
    namespace Vector
    {
        
        
        /// <summary>
        /// Remove all occurence of value from a std::vector
        /// Does NOT maintain order, in fact it replaces the removed value
        /// with the first value at the end of the container wich differs from the one being removed
        /// </summary>
        /// <returns>Count of value removed</returns>
        template <typename T>
        int Remove(T value, std::vector<T>& container);

        /// <summary>
        /// Push a value in the std::vector only if it is not already present
        /// </summary>
        /// <returns>True if value hase been pushed</returns>
        template <typename T>
        bool PushIfDoesntContains(const T& value, std::vector<T>& container);


        /// <returns>True if value has been found</returns>
        template <typename T>
        bool Contains(const T& value, std::vector<T>& container);
       
    }
}

#include "Vector.tpp"