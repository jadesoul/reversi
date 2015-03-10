#ifndef TIMER_H_1421849975_26
#define TIMER_H_1421849975_26
/**
 * File: timer.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:19:35.264778
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include <limits>
#include <ctime>

class timer {
public:
	timer() {
		_start_time = std::clock();
	}

	inline void restart() {
		_start_time = std::clock();
	}

	// return elapsed time in seconds
	inline double elapsed() const {
		return double(std::clock() - _start_time) / CLOCKS_PER_SEC;
	}

	// return estimated maximum value for elapsed()
	// Portability warning: elapsed_max() may return too high a value on systems
	// where std::clock_t overflows or resets at surprising values.
	inline double elapsed_max() const {
		return (double((std::numeric_limits<std::clock_t>::max)())
				- double(_start_time)) / double(CLOCKS_PER_SEC);
	}

	// return minimum value for elapsed()
	inline double elapsed_min() const {
		return double(1) / double(CLOCKS_PER_SEC);
	}

private:
	std::clock_t _start_time;
};

#endif /* TIMER_H_1421849975_26 */
