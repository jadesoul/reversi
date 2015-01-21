#ifndef SCORE_H_1421851913_19
#define SCORE_H_1421851913_19
/**
 * File: score.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:51:53.190950
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "common.h"

//代表一盘棋结束时的比分
class Score {
public:
	uchar black;//黑方棋子数
	uchar white;//白方棋子数
	uchar empty;//未下棋子数
	color winner;//胜利方，取值为 BLACK, WHITE, DRAW 平局

	Score(Board& board) {//游戏结束时，将board传入构造对象
		black=board.total[BLACK];
		white=board.total[WHITE];
		empty=board.total[EMPTY];
		if (black>white) winner=BLACK;
		else if (black<white) winner=WHITE;
		else winner=DRAW;
	}

	inline int diff() {
		return (int)black-(int)white;
	}

	inline int win(color turn) {
		if (turn == BLACK)
			return diff();
		else
			return -diff();
	}

	//turn赢返回1，输返回-1，平局返回0
	inline int sign(color turn) {
		if (winner==turn) return 1;
		if (winner!=DRAW) return -1;
		//must be DRAW
		return 0;
	}

	ostream& dump(ostream& o=cout) {
		o<<"BLACK:"<<(uint)black<<" WHITE:"<<(uint)white<<" EMPTY:"<<(uint)empty<<" ";
		if (winner==BLACK) o<<"BLACK WIN";
		else if (winner==WHITE) o<<"WHITE WIN";
		else o<<"DRAW GAME";
		return o;
	}

	friend inline ostream& operator<<(ostream& o, Score& s) {
		return s.dump(o);
	}
};

#endif /* SCORE_H_1421851913_19 */
