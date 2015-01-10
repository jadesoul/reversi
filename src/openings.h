#ifndef OPENINGS_H_1372388367_57
#define OPENINGS_H_1372388367_57
/**
 * File: openings.h
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2013-06-28 10:59:27.571000
 * Written In: Peking University, beijing, China 
 */

//开局库

//自动产生开局库的想法：
//对游戏的前若干步棋（如前中央16-4=12步棋，或者占边之前）
//使用蒙特卡罗的策略搜索出最佳走法，用数据库记录下来
//注意棋盘的对称性，规模可以折半

//参考开局策略：http://www.othello-club.de/strategy3.php?lang=en

//D3c3C4c5F6                               Buffalo, Kenichi Variation (t3)
//大写：黑子
//小写：白子

//a-z A-Z 水平方向 y
//1-8 垂直方向 x

//对称性：x-> 7-x  y-> 7-y

#include "board.h"
#include "player.h"

class OpeningBook {
public:
	unordered_map<Board, set<move_t> > book;

	OpeningBook() {
		load("../data/");
	}

	//从文件中加载开局库，根据字符串转换为棋局，自动做对称变换
	void load(const char* fp) {
		ifstream fin(fp);
		string s;
		while (fin>>s) {
			HumanPlayer black, white;
			Game game(black, white);
			game.start_one_opening(s, book);
		}
		log_info("loaded opening book: "<<book.size()<<" entries");
	}
};

#endif /* OPENINGS_H_1372388367_57 */
