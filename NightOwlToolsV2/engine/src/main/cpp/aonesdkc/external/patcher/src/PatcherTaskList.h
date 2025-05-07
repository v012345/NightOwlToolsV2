#ifndef _PATCHER_TASK_LIST_H_
#define _PATCHER_TASK_LIST_H_

#include <memory>
#include <list>
#include <map>
#include <condition_variable>
#include <thread>
#include "def.h"

namespace AonePatcher
{
	class IPatcherTask;
	typedef std::shared_ptr<IPatcherTask> IPatcherTaskPtr;

    /**
     This class is manager all the download tasks.
     */
	class PatcherTaskList{
	public:
        
        /**
         Set the max download thread numbers.
         */
		void SetMaxSize(uint32 maxSize);
		void Add(IPatcherTaskPtr task);
		void Remove(IPatcherTaskPtr task);
		bool IsFull();
		bool IsEmpty();
		uint32 GetCount();
		void WaitNotFull();
		void Cancel();
		void Join();
		void Clear();
	private:
		bool IsFullNoLock();
		std::list<IPatcherTaskPtr> m_taskList;
		std::map<std::string, std::thread> m_threadMap;
		std::mutex m_mutex;
		std::condition_variable_any m_notFull;
		uint32 m_maxSize;
	};
}

#endif // !PATCHER_TASK_LIST_H_
