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

#include <iostream>
#include "datetime.h"

//#define LOG_BEGIN char now[128]; get_now(now);
#define LOG_BEGIN
#define LOG_SPLITER ' '

// #define LOG_SINATURE APP_NAME <<' '<<now<<' '
//#define LOG_SINATURE now<<' '
#define LOG_SINATURE ""

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

/* Logging System Version 2 : level can not change in runtime, but faster */

#define log_debug(...) { LOG_BEGIN; std::clog<<LOG_SINATURE<<LOG_MODE_DEBUG<<LOG_SPLITER<< __VA_ARGS__ <<std::endl ; }
#define log_info(...) { LOG_BEGIN; std::clog<<LOG_SINATURE<<LOG_MODE_INFO<<LOG_SPLITER<< __VA_ARGS__ <<std::endl ; }
#define log_status(...) { LOG_BEGIN; std::clog<<LOG_SINATURE<<LOG_MODE_STATUS<<LOG_SPLITER<< __VA_ARGS__ <<std::endl ; }
#define log_warn(...) { LOG_BEGIN; std::clog<<LOG_SINATURE<<LOG_MODE_WARN<<LOG_SPLITER<< __VA_ARGS__ <<std::endl ; }
#define log_error(...) { LOG_BEGIN; std::cerr<<LOG_SINATURE<<LOG_MODE_ERROR<<" Code:"<<__FILE__<<":"<<__LINE__<<" Function:"<<__FUNCTION__<<" Message:"<< __VA_ARGS__ <<std::endl ; }
#define die_error(...) { LOG_BEGIN; std::cerr<<LOG_SINATURE<<LOG_MODE_ERROR<<" Code:"<<__FILE__<<":"<<__LINE__<<" Function:"<<__FUNCTION__<<" Message:"<< __VA_ARGS__ <<std::endl ; exit(-1); }
#define log_fatal(...) { LOG_BEGIN; std::cerr<<LOG_SINATURE<<LOG_MODE_FATAL<<" Code:"<<__FILE__<<":"<<__LINE__<<" Function:"<<__FUNCTION__<<" Message:"<< __VA_ARGS__ <<std::endl ; }
#define die_fatal(...) { LOG_BEGIN; std::cerr<<LOG_SINATURE<<LOG_MODE_FATAL<<" Code:"<<__FILE__<<":"<<__LINE__<<" Function:"<<__FUNCTION__<<" Message:"<< __VA_ARGS__ <<std::endl ; exit(-2); }

//#define LOG_LEVEL LOG_LEVEL_DEBUG
//#define LOG_LEVEL LOG_LEVEL_INFO
#define LOG_LEVEL LOG_LEVEL_STATUS
//#define LOG_LEVEL LOG_LEVEL_WARN
//#define LOG_LEVEL LOG_LEVEL_ERROR
//#define LOG_LEVEL LOG_LEVEL_FATAL
//#define LOG_LEVEL LOG_LEVEL_NONE

#if defined(LOG_LEVEL) && (LOG_LEVEL<LOG_LEVEL_DEBUG)
    #undef log_debug
	#define log_debug(...)
#endif

#if defined(LOG_LEVEL) && (LOG_LEVEL<LOG_LEVEL_INFO)
    #undef log_info
	#define log_info(...)
#endif

#if defined(LOG_LEVEL) && (LOG_LEVEL<LOG_LEVEL_STATUS)
    #undef log_status
	#define log_status(...)
#endif

#if defined(LOG_LEVEL) && (LOG_LEVEL<LOG_LEVEL_WARN)
    #undef log_warn
	#define log_warn(...)
#endif

#if defined(LOG_LEVEL) && (LOG_LEVEL<LOG_LEVEL_ERROR)
    #undef log_error
    #undef die_error
	#define log_error(...)
	#define die_error(...)
#endif

#if defined(LOG_LEVEL) && (LOG_LEVEL<LOG_LEVEL_FATAL)
    #undef log_fatal
    #undef die_fatal
	#define log_fatal(...)
	#define die_fatal(...)
#endif


#endif /* LOGGING_H_1357720107_79 */
