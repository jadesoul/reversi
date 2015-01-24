/**
 * File: score.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:51:57.207267
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "score.h"


Score::Score(Board& board) {//游戏结束时，将board传入构造对象
	black=board.black_cnt();
	white=board.white_cnt();
	empty=board.empty_cnt();
	if (black>white) winner=BLACK;
	else if (black<white) winner=WHITE;
	else winner=DRAW;
}

int Score::sign(color turn) {
	if (winner==turn) return 1;
	if (winner!=DRAW) return -1;
	//must be DRAW
	return 0;
}

ostream& Score::dump(ostream& o) {
	o<<"BLACK:"<<(uint)black<<" WHITE:"<<(uint)white<<" EMPTY:"<<(uint)empty<<" ";
	if (winner==BLACK) o<<"BLACK WIN";
	else if (winner==WHITE) o<<"WHITE WIN";
	else o<<"DRAW GAME";
	return o;
}
