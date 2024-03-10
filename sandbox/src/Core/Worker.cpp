#include "pch.h"
#include "Sandbox/Core/Worker.h"

namespace Sandbox
{
	WorkerThread::WorkerThread() :
		m_currentQueue(false),
		m_threadRunning(false),
		m_taskAvailable(false)
	{

	}
	WorkerThread::~WorkerThread()
	{
		m_threadRunning = false;
		std::unique_lock lock(m_waiterMutex);
		m_taskAvailable = true;
		lock.unlock();
		m_waiter.notify_one();
		m_thread.join();
	}

	void WorkerThread::StartThread()
	{
		m_threadRunning = true;
		m_thread = std::move(std::thread(&WorkerThread::Thread, this));
	}

	void WorkerThread::StopThread()
	{
		m_threadRunning = false;
	}

	void WorkerThread::QueueTask(sptr<OpaqueTask> task)
	{
		std::unique_lock queueLock(m_queueMutex[!m_currentQueue]);
		std::unique_lock waiterLock(m_waiterMutex);

		m_queue[!m_currentQueue].emplace_back(task);

		m_taskAvailable = true;

		queueLock.unlock();
		waiterLock.unlock();

		m_waiter.notify_one();
	}

	void WorkerThread::Thread()
	{
		while (m_threadRunning)
		{
			std::unique_lock waiterLock(m_waiterMutex);
			m_waiter.wait(waiterLock, [this] {return m_taskAvailable; });
			m_currentQueue = !m_currentQueue;
			m_taskAvailable = false;
			waiterLock.unlock();

			std::lock_guard queueLock(m_queueMutex[m_currentQueue]);

			for (auto& task : m_queue[m_currentQueue])
			{
				task->Perform();
			}

			m_queue[m_currentQueue].clear();
		}
	}
}

