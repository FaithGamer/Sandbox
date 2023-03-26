#pragma once

template<int N>
struct my_placeholder { static my_placeholder ph; };

template<int N>
my_placeholder<N> my_placeholder<N>::ph;

namespace std {
	template<int N>
	struct is_placeholder<::my_placeholder<N>> : std::integral_constant<int, N> { };
}

template<class R, class T, class...Types, class U, int... indices>
std::function<R(Types...)> bind_first(std::function<R(T, Types...)> f, U val, std::integer_sequence<int, indices...> /*seq*/) {
	return std::bind(f, val, my_placeholder<indices + 1>::ph...);
}
template<class R, class T, class...Types, class U>
std::function<R(Types...)> bind_first(std::function<R(T, Types...)> f, U val) {
	return bind_first(f, val, std::make_integer_sequence<int, sizeof...(Types)>());
}

namespace Sandbox
{

	/// @brief Holds function pointer to either free function of member function
	/// @tparam ...Args 
	/// @tparam Ret 
	template<typename Ret, typename... Args>
	class Delegate
	{

	public:
		Delegate(Ret(*function)(Args...))
		{
			m_callable = makesptr<FreeFunction>(function);
		}
		template <typename... Ts>
		Delegate(Ret(*function)(Args...), Ts&&... args)
			: m_args(std::forward<Ts>(args)...)
		{
			m_callable = makesptr<FreeFunction>(function);
		}

		template<typename Obj>
		Delegate(Ret(Obj::* function)(Args...))
		{
			m_callable = makesptr<MemberFunction<Obj>>(function, nullptr);
		}

		template<typename Obj>
		Delegate(Ret(Obj::* function)(Args...), Obj* object)
		{
			m_callable = makesptr<MemberFunction<Obj>>(function, object);
		}


		/// @brief Call the function with arguments provided in the delegate's constructor.
		/// If the delegate function is a member function, the function will be called upon the object provided in constructor
		/// The behaviour is undefined if no arguments were provided in the delegate's constructor
		/// @return return of the function
		Ret Call()
		{
			return[this] <std::size_t... I>
				(std::index_sequence<I...>)
			{
				return m_callable->Call(std::forward<Args>(std::get<I>(m_args))...);
			}
			(std::make_index_sequence<sizeof...(Args)>());
		}

		/// @brief Call the function with arguments.
		/// If the delegate function is a member function, the function will be called upon the object provided in constructor
		/// @param ...args arguments
		/// @return return of the function
		Ret Call(Args&&... args)
		{
			return m_callable->Call(std::forward<Args>(args)...);
		}

		/// @brief Call the member function on object with arguments.
		/// @param object object to call the member function upon
		/// @param ...args arguments
		/// @return return of the function
		Ret Call(void* const object, Args&&... args)
		{
			return m_callable->CallWithObject(object, std::forward<Args>(args)...);
		}

		/// @brief Call the member function on object with arguments provided in the delegate's constructor.
		/// The behaviour is undefined if no arguments were provided in the delegate's constructor
		/// @param object object to call the member function upon
		/// @return return of the function
		Ret Call(void* const object)
		{
			return[this] <std::size_t... I>
				(std::index_sequence<I...>)
			{
				return m_callable->CallWithObject(object, std::forward<Args>(std::get<I>(m_args))...);
			}
			(std::make_index_sequence<sizeof...(Args)>());
		}

		bool IsSameFunction(Ret(*function)(Args...)) const
		{
			return m_callable->IsSameFunction(function);
		}

	private:

		struct Callable
		{
			virtual ~Callable() {};
			virtual Ret CallWithObject(void* const object, Args&&... args) = 0;
			virtual Ret Call(Args&&... args) = 0;
			virtual bool IsSameFunction(Ret(*function)(Args...)) const = 0;
		};

		template <typename Obj>
		struct MemberFunction : Callable
		{
			MemberFunction(Ret(Obj::* method)(Args...), Obj* object)
				: m_function(method), m_object(object)
			{

			}
			MemberFunction(Ret(Obj::* method)(Args...))
				: m_function(method), m_object(nullptr)
			{

			}
			Ret CallWithObject(void* const object, Args&&... args) override
			{
				return (static_cast<Obj*>(object)->*m_function)(std::forward<Args>(args)...);
			}
			Ret Call(Args&&... args) override
			{
				return (m_object->*m_function)(std::forward<Args>(args)...);
			}
			bool IsSameFunction(Ret(*function)(Args...)) const override
			{
				return false;
			}

			Ret(Obj::* m_function)(Args...);
			Obj* m_object;
		};

		struct FreeFunction : Callable
		{
			FreeFunction(Ret(*function)(Args...))
				: m_function(function)
			{

			}
			FreeFunction(Ret(*function))
				: m_function(function)
			{

			}
			Ret Call(Args&&... args) override
			{
				return (*m_function)(std::forward<Args>(args)...);
			}
			Ret CallWithObject(void* const object, Args&&... args) override
			{
				//no object of free function
			}
			bool IsSameFunction(Ret(*function)(Args...)) const override
			{
				return m_function == function;
			}

			Ret(*m_function)(Args...);
		};


		sptr<Callable> m_callable;
		std::tuple<Args...> m_args;
	};
}