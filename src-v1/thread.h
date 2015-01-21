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

#include "bootstrap.h"
#include "logging.h"

#ifdef OS_WIN32
#include <process.h>
#else
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


thread::thread() {
	this->name="thread";
	this->thread_id=0;
	this->job_id=0;
	this->verbose=LOG_LEVEL_DEBUG;

	running = false;
#ifdef OS_WIN32
	InitializeCriticalSection(&critSec);
#else
	pthread_mutex_init(&myMutex, NULL);
#endif
}

thread::~thread() {
#ifndef OS_WIN32
	pthread_mutex_destroy(&myMutex);
#endif
	if (running) stop();
}

bool thread::start(const string& name, uint thread_id, uint job_id, int verbose, bool blocking) {
	this->name=name;
	this->thread_id=thread_id;
	this->job_id=job_id;
	this->verbose=verbose;
	ostringstream oss;
	oss<<name<<"("<<thread_id<<"-"<<job_id<<"): ";
	this->prefix=oss.str();

	if (running) {
		log_debug(prefix<<"thread already running");
		return false;
	}
	// have to put this here because the thread can be running
	// before the call to create it returns
	running   = true;
#ifdef OS_WIN32
	// InitializeCriticalSection(&critSec);
	// unsigned long _beginthreadex(
		// void *security,
		// unsigned stack_size,
		// unsigned ( __stdcall *start_address )( void * ),
		// void *arglist,
		// unsigned initflag,
		// unsigned *thrdaddr
	// );
	myThread = (HANDLE)_beginthreadex(NULL, 0, this->run,  (void *)this, 0, NULL);
#else
	// pthread_mutex_init(&myMutex, NULL);
	pthread_create(&myThread, NULL, run, (void *)this);
#endif
	this->blocking=blocking;
	log_debug(prefix<<"thread started");
	return true;
}

bool thread::lock() {	//returns false if it can't lock
#ifdef OS_WIN32
	if (blocking) EnterCriticalSection(&critSec);
	else {
		if (!TryEnterCriticalSection(&critSec)) {
			log_debug(prefix<<"mutext is busy");
			return false;
		}
	}
	log_debug(prefix<<"we are in -- mutext is now locked");
#else
	if (blocking) {
		log_debug(prefix<<"waiting till mutext is unlocked");
		pthread_mutex_lock(&myMutex);
		log_debug(prefix<<"we are in -- mutext is now locked");
	} else {
		int value=pthread_mutex_trylock(&myMutex);
		if (value==0) {
			log_debug(prefix<<"we are in -- mutext is now locked");
		}
		else {
			log_debug(prefix<<"mutext is busy - already locked");
			return false;
		}
	}
#endif
	return true;
}

bool thread::unlock() {
#ifdef OS_WIN32
	LeaveCriticalSection(&critSec);
#else
	pthread_mutex_unlock(&myMutex);
#endif
	log_debug(prefix<<"we are out -- mutext is now unlocked");
	return true;
}

void thread::stop() {
	if (running) {
		running=false;
#ifdef OS_WIN32
		CloseHandle(myThread);
#else
		pthread_detach(myThread);
#endif
		log_debug(prefix<<"thread stopped");
	} else {
		log_debug(prefix<<"thread already stopped");
	}
}

void thread::run() {
	log_debug(prefix<<"please overide this run method for thead main code");
}

#ifdef OS_WIN32
unsigned int __stdcall thread::run(void * ptr) {
	thread* me=(thread*)ptr;
	me->run();
	me->stop();
	return 0;
}
#else
void* thread::run(void * ptr) {
	thread* me=(thread*)ptr;
	me->run();
	me->stop();
	return 0;
}
#endif

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
