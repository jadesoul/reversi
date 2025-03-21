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

//沿着两条对角线对称：1个开局可以对称出4个
//对称性1：x-> 7-x  y-> 7-y
//对称性2：x<-y y<-x

#include "board.h"
#include "player.h"

class OpeningBook {
public:
	map<Board, Choices> book;
	Random random;

	OpeningBook() {
		load("/Users/jadesoul/git/reversi/data/openingslarge.txt");
		load("/Users/jadesoul/git/reversi/data/openings.txt");
	}

	//从文件中加载开局库，根据字符串转换为棋局，自动做对称变换
	void load(const char* fp) {
		ifstream fin(fp);
		log_status("loading opening book from: "<<fp);
		string s;
		uint cnt=0;
		while (getline(fin, s)) {
			HumanPlayer black, white;
			Game game(black, white);
			if (game.start_one_opening(s, book)) {
				++cnt;
			}
		}

		uint total_moves;
		for (map<Board, Choices>::iterator it=book.begin(); it!=book.end(); ++it) {
			total_moves+=it->second.size();
		}
		log_status("loaded opening book: "<<cnt<<" openings, now "<<book.size()<<" boards, "<<total_moves<<" moves");
		fin.close();
	}

	move_t lookup(const Board& board) const {
		map<Board, Choices>::const_iterator it=book.find(board);
		if (it==book.end()) {
			return PASS;
		} else {

			const Choices& moves=it->second;
			uint n=moves.size();
			assert(n>0);
			log_status(board);

			move_t move=PASS;
			uint cnt=0;
			double max_score=INT32_MIN;
			for (Choices::const_iterator it2=moves.begin(); it2!=moves.end(); ++it2) {
				double score=it2->second;
				if (score>max_score) {
					max_score=score;
					move=it2->first;
				}
				++cnt;
				log_status("found opening "<<cnt<<" : "<<Move(it2->first, board.turn));
			}

//			if (n==1) {//只有1种方案
//				move= moves.begin()->first;
//			} else {//多种方案，随机选择一个结果
//				uint index=random.randindex(cnt);
//				Choices::const_iterator it3=moves.begin();
//				while (index-- > 0) {
//					++it3;
//				}
//				move= it3->first;
//			}

			log_status("found "<<n<<" opening moves in opening book, choose move: "<<Move(move, board.turn)<<", score="<<max_score);
			return move;
		}
		return PASS;
	}
};

extern OpeningBook* openings;

#endif /* OPENINGS_H_1372388367_57 */
