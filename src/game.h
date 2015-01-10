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
		if (turn==BLACK) return diff();
		else return -diff();
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

class Game {
public:
	Board board;
	
	Player& black;
	Player& white;
	
	inline bool game_over() { //无子可下，或者连续两次PASS
		return board.game_over();
	}
	
	Game(Player& black, Player& white):
		black(black), white(white) {}
	Game(Player& black, Player& white, Board& board):
		black(black), white(white), board(board) {}
	
	Score start() {
		log_info("Game Start!!");
		black.reset();
		white.reset();
		while (!game_over()) {
			color& turn=board.turn;
			Player& player=(turn==BLACK)?black:white;
			uchar& mobility=board.mobility();
			
			if (mobility==0) {
				board.pass();
			} else {
				player.play(board);
			}
		}

		log_info("Game Over!!");
		
		log_debug(board);
//		log_status(board);

		Score score(board);
		log_info(score);
		
		return score;
	}
	
	//用于加载开局库
	void start_one_opening(const string& opening, unordered_map<Board, set<move_t> >& book) {
		log_info("Game for Opening Start!!");
		black.reset();
		white.reset();
		for (uint i=0; i<opening.size(); i+=2) {
			color& board_turn=board.turn;

			const char* two_bytes=opening.c_str()+i;
			uchar move, x, y;
			color turn;
			bool ok = parse_move(two_bytes, move, x, y, turn);
			if (!ok) die_error("illegal opening:"<<opening<<" two_bytes from:"<<two_bytes);
			assert(turn==board_turn);
			assert(board.mobility()!=0);
			assert(board.is_active(x, y));

			unordered_map<Board, set<move_t> >::iterator it=book.find(board);
			if (it==book.end()) {
				log_debug("add new book entry");
				set<move_t> moves;
				moves.insert(move);
				book[board]=moves;
			} else {
				log_debug("expand exist book entry");
				set<move_t>& moves=it->second;
				moves.insert(move);
			}
			uint eat=board.play(move);
			assert(eat>0);
		}
		log_info("Game for Opening Stop!!");
	}


	//用于游戏引擎，给定字符串（64字符的游戏局面和1个字符的turn）
	//返回下子的位置坐标 (2个字符)，下标均是从1开始计算
	string deal(const string& query) {
		board.init_from_str(query);
		color& turn=board.turn;
		Player& player=(turn==BLACK)?black:white;
		uchar& mobility=board.total[ACTIVE];
		
		if (mobility==0) {
			return "00";//表示PASS
		} else {
			uchar byte=player.play(board);
			uchar x=byte>>4, y=byte&0x0F;
			assert(x<8 and y<8);
			char s[3];
			s[0]='1'+x;//从1开始编号
			s[1]='1'+y;
			s[2]=0;
			return s;
		}
	}
};

#endif /* GAME_H_1371896260_1 */

