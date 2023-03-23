#include "pch.h"
#include "Core/Signals.h"

namespace Sandbox
{
	//////////////////////////////////
	/// SignalDispatcher
	//////////////////////////////////

	void SignalDispatcher::RemoveListener(void* const listener)
	{

		for (auto& signal : m_delegates)
		{
			auto& listeners = signal.second;
			for (auto listenerSignalPriority = listeners.begin(); listenerSignalPriority != listeners.end();)
			{
				if (listenerSignalPriority->listener == listener)
				{
					listeners.erase(listenerSignalPriority++);
				}
				else
				{
					listenerSignalPriority++;
				}
			}

		}
	}

	//////////////////////////////////
	/// SignalSender
	//////////////////////////////////

	void SignalSender::RemoveListener(void* listener)
	{
		m_dispatcher.RemoveListener(listener);
	}
}