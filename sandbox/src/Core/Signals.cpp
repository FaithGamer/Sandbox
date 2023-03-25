#include "pch.h"
#include "Core/Signals.h"

namespace Sandbox
{
	//////////////////////////////////
	/// SignalDispatcher
	//////////////////////////////////

	void SignalDispatcher::RemoveListenerFromAll (void* const listener)
	{

		for (auto& signal : m_callbacks)
		{
			auto& listeners = signal.second;
			for (auto functionPriority = listeners.begin(); functionPriority != listeners.end();)
			{
				if (functionPriority->listener == listener)
				{
					listeners.erase(functionPriority++);
				}
				else
				{
					functionPriority++;
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