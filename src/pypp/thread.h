#ifndef THREAD_H_1356505929_76
#define THREAD_H_1356505929_76
/**
 * File: thread.h
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2012-12-26 15:12:09.756000
 * Written In: Peking University, beijing, China
 */

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>

#include "macros.h"
#include "logging.h"

using std::string;
using std::vector;
using std::list;
using std::ostringstream;

#ifdef OS_WIN32
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#endif

class thread {
protected:
#ifdef OS_WIN32
	HANDLE            myThread;
	CRITICAL_SECTION  critSec;  	//same as a mutex
#else
	pthread_t        myThread;
	pthread_mutex_t  myMutex;
#endif
	string name;			//thread name
	uint thread_id;			//thread id
	uint job_id;			//job id
	string prefix;			//for debug
	int verbose;
	bool running;
	bool blocking;
	
public:
	thread();
	virtual ~thread();
	
	inline bool isrunning() const { return running; }
	bool start(const string& name="thread", uint thread_id=0, uint job_id=0, int verbose=LOG_LEVEL_NONE, bool blocking=false);
	bool lock();
	bool unlock();
	void stop();

protected:
	virtual void run();
#ifdef OS_WIN32
	static unsigned int __stdcall run(void * ptr);
#else
	static void* run(void * ptr);
#endif
};

template<class Thread>
void parallel(list<uint> job_ids, uint total_threads=10, int verbose=LOG_LEVEL_NONE, const string& name="thread") {
	vector<Thread*> threads;
	for_n(i, total_threads) threads.push_back(new Thread);
	
	while (true) {
		bool all_finished=true;
		for_n(thread_id, total_threads) {
			Thread* t=threads[thread_id];
			if (t->isrunning()) {
				all_finished=false;
			} else {
				if (job_ids.empty()) continue;
				
				uint job_id=job_ids.front();
				job_ids.pop_front();
				t->start(name, thread_id, job_id, verbose);
				all_finished=false;
			}
		}
		if (all_finished)
			break;
		usleep(1000*1000/5);
	}
	for_n(i, total_threads) delete threads[i];
}


#endif /* THREAD_H_1356505929_76 */
