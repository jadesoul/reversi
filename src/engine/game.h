#ifndef GAME_H_1371896260_1
#define GAME_H_1371896260_1
/**
 * File: game.h
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2013-06-22 18:17:40.104000
 * Written In: Peking University, beijing, China
 */

#include "player.h"
#include "score.h"

//Game用来控制游戏结束逻辑：如果有连续两次PASS，则游戏结束

class Game {
private:
	Player& black;
	Player& white;
	Board board;
	uint pass_cnt;
	
public:
	Game(Player& self):
		black(self), white(self), pass_cnt(0) {}
	Game(Player& black, Player& white):
		black(black), white(white), pass_cnt(0) {}
	Game(Player& black, Player& white, Board& board):
		black(black), white(white), board(board), pass_cnt(0) {}
	
	//无子可下，或者连续两次PASS
	inline bool game_over() { return board.empty_cnt()==0 or pass_cnt>=2; }

	//开始一盘游戏，直到结束，返回比分对象
	Score start();

	//用于加载开局库
	bool start_one_opening(const string& opening, map<Board, Choices>& book);

	//用于蒙特卡洛扩展开局库
	bool start_expand_opening(map<Board, Choices>& book);

	//用于游戏引擎，给定字符串（64字符的游戏局面和1个字符的turn）
	//返回下子的位置坐标 (2个字符)，下标均是从1开始计算
	string deal(const string& query);
};

#endif /* GAME_H_1371896260_1 */

