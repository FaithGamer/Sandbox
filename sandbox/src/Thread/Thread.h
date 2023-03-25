#pragma once

#include <thread>
#include <condition_variable>
#include <std_macros.h>



namespace Sandbox
{
	struct TaskData
	{
		int data;
	};

	/// @brief Run given task within it's own thread
	///Thread running only when tasks available in the queue, adding tasks to the queue is not blocking
	class WorkerThread
	{
	public:
		WorkerThread();
		~WorkerThread();

		void StartThread();
		void StopThread();
		void QueueTask(sptr<TaskData> taskdata);

	private:
		void Thread();

	protected:

		virtual void Task(sptr<TaskData> taskData);


	private:

		std::condition_variable m_waiter;
		std::mutex m_queueMutex[2];
		std::mutex m_waiterMutex;
		std::vector<sptr<TaskData>> m_queue[2];
		std::thread m_thread;
		bool m_currentQueue;
		bool m_taskAvailable;
		std::atomic<bool> m_threadRunning;
	};
}