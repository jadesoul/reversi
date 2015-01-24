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
#include "board.h"

//代表一盘棋结束时的比分
class Score {
public:
	uchar black;//黑方棋子数
	uchar white;//白方棋子数
	uchar empty;//未下棋子数
	color winner;//胜利方，取值为 BLACK, WHITE, DRAW 平局

	//游戏结束时，将board传入构造对象
	Score(Board& board);

	inline int diff() { return (int)black-(int)white; }

	inline int win(color turn) { return (turn == BLACK) ? diff() : -diff(); }

	//turn赢返回1，输返回-1，平局返回0
	int sign(color turn);

	ostream& dump(ostream& o=cout);
	friend inline ostream& operator<<(ostream& o, Score& s) { return s.dump(o); }
};

#endif /* SCORE_H_1421851913_19 */
