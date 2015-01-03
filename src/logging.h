#ifndef LOGGING_H_1357720107_79
#define LOGGING_H_1357720107_79
/**
 * File: logging.h
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2013-01-09 16:28:27.789601
 * Written In: Peking University, beijing, China
 */

#include "bootstrap.h"
#include <time.h>
extern uint verbose;

void get_now(char out[]) {
	time_t now;
	struct tm *tm_now;
	time(&now);
	tm_now = localtime(&now);
	sprintf(out, "%d-%d-%d %d:%d:%d", tm_now->tm_year + 1900,
			tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour,
			tm_now->tm_min, tm_now->tm_sec);
}

#define LOG_BEGIN char now[128]; get_now(now);
#define LOG_SPLITER ' '

// #ifndef APP_NAME
// #define APP_NAME "System"
// #endif

// #define LOG_SINATURE APP_NAME <<' '<<now<<' '
#define LOG_SINATURE now<<' '
#define LOG_MODE_DEBUG "[DEBUG]"
#define LOG_MODE_INFO "[ INFO]"
#define LOG_MODE_STATUS "[STATS]"
#define LOG_MODE_WARN "[ WARN]"
#define LOG_MODE_ERROR "[ERROR]"
#define LOG_MODE_FATAL "[FATAL]"

#define LOG_LEVEL_DEBUG 6
#define LOG_LEVEL_INFO 5
#define LOG_LEVEL_STATUS 4
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_FATAL 1
#define LOG_LEVEL_NONE 0

#define log_debug(...) if (verbose>=LOG_LEVEL_DEBUG) { LOG_BEGIN; std::clog<<LOG_SINATURE<<LOG_MODE_DEBUG<<LOG_SPLITER<< __VA_ARGS__ <<std::endl ; }
#define log_info(...) if (verbose>=LOG_LEVEL_INFO) { LOG_BEGIN; std::clog<<LOG_SINATURE<<LOG_MODE_INFO<<LOG_SPLITER<< __VA_ARGS__ <<std::endl ; }
#define log_status(...) if (verbose>=LOG_LEVEL_STATUS) { LOG_BEGIN; std::clog<<LOG_SINATURE<<LOG_MODE_STATUS<<LOG_SPLITER<< __VA_ARGS__ <<std::endl ; }
#define log_warn(...) if (verbose>=LOG_LEVEL_WARN) { LOG_BEGIN; std::clog<<LOG_SINATURE<<LOG_MODE_WARN<<LOG_SPLITER<< __VA_ARGS__ <<std::endl ; }
#define log_error(...) if (verbose>=LOG_LEVEL_ERROR) { LOG_BEGIN; std::cerr<<LOG_SINATURE<<LOG_MODE_ERROR<<" Code:"<<__FILE__<<":"<<__LINE__<<" Function:"<<__FUNCTION__<<" Message:"<< __VA_ARGS__ <<std::endl ; }
#define die_error(...) { if (verbose>=LOG_LEVEL_ERROR) { LOG_BEGIN; std::cerr<<LOG_SINATURE<<LOG_MODE_ERROR<<" Code:"<<__FILE__<<":"<<__LINE__<<" Function:"<<__FUNCTION__<<" Message:"<< __VA_ARGS__ <<std::endl ; } exit(-1); }
#define log_fatal(...) if (verbose>=LOG_LEVEL_FATAL) { LOG_BEGIN; std::cerr<<LOG_SINATURE<<LOG_MODE_FATAL<<" Code:"<<__FILE__<<":"<<__LINE__<<" Function:"<<__FUNCTION__<<" Message:"<< __VA_ARGS__ <<std::endl ; }
#define die_fatal(...) if (verbose>=LOG_LEVEL_FATAL) { LOG_BEGIN; std::cerr<<LOG_SINATURE<<LOG_MODE_FATAL<<" Code:"<<__FILE__<<":"<<__LINE__<<" Function:"<<__FUNCTION__<<" Message:"<< __VA_ARGS__ <<std::endl ; exit(-2); }

// #define log_debug(...) if (verbose!=LOG_LEVEL_NONE and verbose<=LOG_LEVEL_DEBUG) { LOG_BEGIN; clog<<LOG_SINATURE<<LOG_MODE_DEBUG<<LOG_SPLITER<< __VA_ARGS__ <<endl ; }
// #define log_info(...) if (verbose!=LOG_LEVEL_NONE and verbose<=LOG_LEVEL_INFO) { LOG_BEGIN; clog<<LOG_SINATURE<<LOG_MODE_INFO<<LOG_SPLITER<< __VA_ARGS__ <<endl ; }
// #define log_status(...) if (verbose!=LOG_LEVEL_NONE and verbose<=LOG_LEVEL_STATUS) { LOG_BEGIN; clog<<LOG_SINATURE<<LOG_MODE_STATUS<<LOG_SPLITER<< __VA_ARGS__ <<endl ; }
// #define log_warn(...) if (verbose!=LOG_LEVEL_NONE and verbose<=LOG_LEVEL_WARN) { LOG_BEGIN; clog<<LOG_SINATURE<<LOG_MODE_WARN<<LOG_SPLITER<< __VA_ARGS__ <<endl ; }
// #define log_error(...) if (verbose!=LOG_LEVEL_NONE and verbose<=LOG_LEVEL_ERROR) { LOG_BEGIN; clog<<LOG_SINATURE<<LOG_MODE_ERROR<<LOG_SPLITER<< __VA_ARGS__ <<endl ; }
// #define die_error(...) { if (verbose!=LOG_LEVEL_NONE and verbose<=LOG_LEVEL_ERROR) { LOG_BEGIN; clog<<LOG_SINATURE<<LOG_MODE_ERROR<<LOG_SPLITER<< __VA_ARGS__ <<endl ; } exit(1); }
// #define log_fatal(...) if (verbose!=LOG_LEVEL_NONE and verbose<=LOG_LEVEL_FATAL) { LOG_BEGIN; clog<<LOG_SINATURE<<LOG_MODE_FATAL<<LOG_SPLITER<< __VA_ARGS__ <<endl ; }

#endif /* LOGGING_H_1357720107_79 */
