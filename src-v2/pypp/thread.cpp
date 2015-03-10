/**
 * File: thread.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:30:58.162098
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "thread.h"

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
