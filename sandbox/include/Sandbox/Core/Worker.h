#pragma once

#include <thread>
#include <condition_variable>

#include "Sandbox/Core/std_macros.h"
#include "Sandbox/Core/Task.h"

namespace Sandbox
{
	
	/// @brief Run given task within it's own thread
	///Thread running only when tasks available in the queue, adding tasks to the queue is not blocking
	class WorkerThread
	{
	public:
		WorkerThread();
		~WorkerThread();

		void StartThread();
		void StopThread();
		void QueueTask(sptr<OpaqueTask> taskdata);

	private:
		void Thread();

	protected:


	private:

		std::condition_variable m_waiter;
		std::mutex m_queueMutex[2];
		std::mutex m_waiterMutex;
		std::vector<sptr<OpaqueTask>> m_queue[2];
		std::thread m_thread;
		bool m_currentQueue;
		bool m_taskAvailable;
		std::atomic<bool> m_threadRunning;
	};


}