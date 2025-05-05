#include "PatcherTaskList.h"
#include "PatcherTask.h"
#include "cplog.h"

namespace AonePatcher
{
	void PatcherTaskList::SetMaxSize(uint32 maxSize)
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		m_maxSize = maxSize;
	}

	void PatcherTaskList::Add(IPatcherTaskPtr task)
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		m_taskList.push_back(task);
		if (m_threadMap.find(task->m_name) != m_threadMap.end()) {
			CP_LOGE("find duplicate url %s", task->m_name.c_str());
		}
		m_threadMap.insert(std::pair<std::string, std::thread>(task->m_name, std::thread([task](){
			task->Do();
		})));
	}

	void PatcherTaskList::Remove(IPatcherTaskPtr task)
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		//if called after Join, thread not in map for deadlock reason
		if (m_threadMap.find(task->m_name) != m_threadMap.end())
		{
			if (m_threadMap[task->m_name].joinable())
			{
				//after remove, outer don't care about thread result, so detach is ok
				m_threadMap[task->m_name].detach();
			}
			m_threadMap.erase(task->m_name);
		}
		m_taskList.remove(task);
		m_notFull.notify_one();
	}

	bool PatcherTaskList::IsFull()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return IsFullNoLock();
	}

	void PatcherTaskList::Clear()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		m_taskList.clear();
		m_threadMap.clear();
	}

	bool PatcherTaskList::IsFullNoLock()
	{
		return m_taskList.size() >= m_maxSize;
	}

	uint32 PatcherTaskList::GetCount()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_taskList.size();
	}

	void PatcherTaskList::WaitNotFull()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		m_notFull.wait(m_mutex, [this]{ return !IsFullNoLock(); });
	}

	void PatcherTaskList::Cancel()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		for (auto& taskPtr : m_taskList)
		{
			taskPtr->Cancel();
		}
	}

	void PatcherTaskList::Join()
	{
		std::list<std::thread> threadList;
		{
			std::lock_guard<std::mutex> locker(m_mutex);
			for (auto& it : m_threadMap)
			{
				threadList.push_back(std::move(it.second));
			}
			m_threadMap.clear();
		}

		for (auto& it : threadList)
		{
			if (it.joinable())
			{
				it.join();
			}
		}
		Clear();
	}

	bool PatcherTaskList::IsEmpty()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_taskList.size() == 0;
	}
}
