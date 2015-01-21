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
			uchar mobility=board.mobility();
			
			if (mobility==0) {
				board.pass();
			} else {
				player.play(board);
			}
		}

		log_info("Game Over!!");
		
//		log_debug(board);
		log_status(board);

		Score score(board);
//		log_status(score);
		
		return score;
	}

	//用于加载开局库
	bool start_one_opening(const string& opening, map<Board, Choices>& book) {
		log_debug("Game for Opening Start!!");
		black.reset();
		white.reset();
		for (uint i=0; i<opening.size(); i+=2) {
			color& board_turn=board.turn;
//			log_warn(board);

			const char* two_bytes=opening.c_str()+i;
			uchar move, x, y;
			color turn;
			bool ok = parse_move(two_bytes, move, x, y, turn);
			if (!ok) {
//				log_warn("illegal opening:"<<opening<<" two_bytes from:"<<two_bytes);
				if (i!=0) return true;
				else return false;
			}
//			assert(turn==board_turn);
			assert(board.mobility()!=0);
			assert(board.is_active(x, y));
//			log_warn("move: "<<Move(move, turn));

			vector<Board> mirror_boards(4, board);//拷贝4份
			mirror_boards[1].mirror_xy();
			mirror_boards[2].mirror_ldru();
			mirror_boards[3].mirror_ldru_xy();

			vector<Pos> mirror_moves(4, Pos(move));
			mirror_moves[1].mirror_xy();
			mirror_moves[2].mirror_ldru();
			mirror_moves[3].mirror_ldru_xy();

			//添加开局走法
			for_n(j, 4) {
				Board& the_board=mirror_boards[j];
				move_t the_move=mirror_moves[j].tomove();

//				log_warn(the_board);
//				log_warn("j="<<j<<" the_move="<<Pos(the_move));

				assert(the_board.mobility()!=0);
				assert(the_board.is_active(the_move));

//				map<Board, set<move_t> >::iterator it = book.find(the_board);
//				if (it == book.end()) {
//					log_debug("add new book entry");
//					set<move_t> moves;
//					moves.insert(the_move);
//					book[the_board] = moves;
//				} else {
//					log_debug("expand exist book entry");
//					set<move_t>& moves = it->second;
//					moves.insert(the_move);
//				}

				book[the_board][the_move]=1;
			}

			uint eat=board.play(move);
			assert(eat>0);

		}
		log_debug("Game for Opening Stop!!");
		return true;
	}

	//用于蒙特卡洛扩展开局库
	bool start_expand_opening(map<Board, Choices>& book) {
		black.reset();
		white.reset();

		vector<pair<Board, move_t> > history;
		while (!game_over()) {

			color& turn = board.turn;
			Player& player = (turn == BLACK) ? black : white;
			uchar mobility = board.mobility();

			if (mobility == 0) {
				board.pass();
			} else {
				history.push_back(make_pair(board, PASS));

				move_t move=player.play(board);
				history.back().second=move;
			}
		}
		Score score(board);
		int diff=score.diff();//黑子数减去白子数之差

		uint n=history.size();
		for_n(i, n) {
			Board& b=history[i].first;
			move_t move=history[i].second;

			color& board_turn=b.turn;
			int my_diff=(board_turn!=BLACK) ? diff : -diff;

			assert(b.mobility()!=0);
			assert(b.is_active(move));

			vector<Board> mirror_boards(4, b);//拷贝4份
			mirror_boards[1].mirror_xy();
			mirror_boards[2].mirror_ldru();
			mirror_boards[3].mirror_ldru_xy();

			vector<Pos> mirror_moves(4, Pos(move));
			mirror_moves[1].mirror_xy();
			mirror_moves[2].mirror_ldru();
			mirror_moves[3].mirror_ldru_xy();

			//添加开局走法
			for_n(j, 4) {
				Board& the_board=mirror_boards[j];
				move_t the_move=mirror_moves[j].tomove();

				assert(the_board.mobility()!=0);
				assert(the_board.is_active(the_move));

				book[the_board][the_move]+=0;
				book[the_board][the_move]=(book[the_board][the_move]+my_diff)*0.45;
			}
		}
		return true;
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

