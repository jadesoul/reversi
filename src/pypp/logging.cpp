/**
 * File: logging.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-24 16:41:41.706405
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "logging.h"

void get_now(char out[]) {
	time_t now;
	struct tm *tm_now;
	time(&now);
	tm_now = localtime(&now);
	sprintf(out, "%d-%d-%d %d:%d:%d", tm_now->tm_year + 1900,
			tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour,
			tm_now->tm_min, tm_now->tm_sec);
}
