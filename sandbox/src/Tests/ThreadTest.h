#pragma once

namespace Sandbox
{
#include <vector>

    template<typename... Args>
    class Delegate {
    public:
        // Constructor that takes the function pointer as an argument
        Delegate(void (*func)(Args...)) : m_function(func) {}

        template<typename... Ts>
        Delegate(void (*func)(Args...), Ts&&... args) : m_function(func), m_args(std::forward<Ts>(args)...) {}

        template<typename Obj, typename... Ts>
        Delegate(Obj* object, void (Obj::*func)(Args...), Ts&&... args) : m_args(std::forward<Ts>(args)...), m_object(object)
        {

        }

        // Call the stored function with the stored arguments
        void Invoke() {
            callImpl(std::make_index_sequence<sizeof...(Args)>{});
        }

        // Store the arguments in the tuple
        template<typename... Ts>
        void setArgs(Ts&&... args) {
            m_args = std::make_tuple(std::forward<Ts>(args)...);
        }

    private:
        // Helper function to call the stored function with the stored arguments
        template<size_t... I>
        void callImpl(std::index_sequence<I...>) {
            m_function(std::get<I>(m_args)...);
        }

        // Function pointer and tuple to store the arguments
        std::function<void(Args...)> m_function;
        std::tuple<Args...> m_args;
        void* m_object;

        
    };


	void ThreadTest();
}
