#pragma once

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <any>
#include <functional>
#include "std_macros.h"


namespace Sandbox
{

	enum class SignalPriority : int
	{
		high = 0,
		medium,
		low
	};

	template<typename SignalType>
	struct SignalCallback
	{
		SignalCallback(std::function<void(const SignalType&)> Callback, void* const Listener, SignalPriority Priority)
			: callback(Callback), listener(Listener), priority(Priority)
		{

		}
		void* const listener;
		std::function<void(const SignalType&)> callback;
		SignalPriority priority;
	};

	template<typename SignalType>
	struct SignalCallbackFreeFunction
	{
		SignalCallbackFreeFunction(std::function<void(const SignalType&)> Callback, SignalPriority Priority)
			: callback(Callback), priority(Priority)
		{

		}
		std::function<void(const SignalType&)> callback;
		SignalPriority priority;
	};

	/// @brief Holds a collection of listeners and their callbacks
	template<typename SignalType>
	class SignalSender
	{
	public:

		template <typename ListenerType>
		void AddListener(void (ListenerType::* callback)(const SignalType&), ListenerType* const listener, SignalPriority priority = SignalPriority::medium);
		void AddListener(void (*callback)(const SignalType&), SignalPriority priority = SignalPriority::medium);


		void RemoveListener(void (*callback)(const SignalType&));
		void RemoveListener(void* const listener);

		void SendSignal(const SignalType& signalData);
	private:
		template<typename SignalType>
		struct CompareCallback
		{
		
			bool operator()(const SignalCallback<SignalType>& l, const SignalCallback<SignalType>& r) const
			{
				return l.priority < r.priority;
			}
		};
		template<typename SignalType>
		struct CompareCallbackFree
		{
	
			bool operator()(const SignalCallbackFreeFunction<SignalType>& l, const SignalCallbackFreeFunction<SignalType>& r) const
			{
				return l.priority < r.priority;
			}
		};
		std::multiset<SignalCallback<SignalType>, CompareCallback<SignalType>> m_listeners;
		std::multiset<SignalCallbackFreeFunction<SignalType>, CompareCallbackFree<SignalType>> m_listenersFreeFunction;

	};
}
#include "Signal.tpp"