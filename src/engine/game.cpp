/**
* File: game.cpp
* Description:
*
* Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
*
* Date: 2015-01-23 13:17:26.139676
* Written In: Alibaba-inc, Hangzhou, China
*/

#include "game.h"

Score Game::start() {
	log_info("Game Start!!");
	black.reset();
	white.reset();
	while (!game_over()) {
		color turn=board.get_current_turn();
		Player& player=(turn==BLACK)?black:white;
		uchar mobility=board.mobility();

		if (mobility==0) {
			board.pass();
		} else if (mobility==1) {
			pos_t p=board.get_first_move();
			assert(p!=PASS);
			board.play(p);
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
bool Game::start_one_opening(const string& opening, map<Board, Choices>& book) {
	log_debug("Game for Opening Start!!");
	black.reset();
	white.reset();
	for (uint i=0; i<opening.size(); i+=2) {
		color board_turn=board.get_current_turn();

		const char* two_bytes=opening.c_str()+i;
		uchar x, y;

		Move move(two_bytes);
		if (! move.is_illegal_move()) {
			log_warn("illegal opening:"<<opening<<" two_bytes from:"<<two_bytes);
			if (i != 0)
				return true;
			else
				return false;
		}
		color turn=move.turn;

//			assert(turn==board_turn);
		assert(board.mobility()!=0);
		assert(board.is_active(x, y));
//			log_warn("move: "<<Move(move, turn));

		vector<Board> mirror_boards(4, board);//拷贝4份
		mirror_boards[1].mirror_xy();
		mirror_boards[2].mirror_ldru();
		mirror_boards[3].mirror_ldru_xy();

		vector<Move> mirror_moves(4, move);
		mirror_moves[1].mirror_xy();
		mirror_moves[2].mirror_ldru();
		mirror_moves[3].mirror_ldru_xy();

		//添加开局走法
		for_n(j, 4) {
			Board& the_board=mirror_boards[j];
			pos_t the_move=mirror_moves[j].pos;

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
bool Game::start_expand_opening(map<Board, Choices>& book) {
	black.reset();
	white.reset();

	vector<pair<Board, pos_t> > history;
	while (!game_over()) {

		color turn = board.get_current_turn();
		Player& player = (turn == BLACK) ? black : white;
		uchar mobility = board.mobility();

		if (mobility == 0) {
			board.pass();
		} else {
			history.push_back(make_pair(board, PASS));

			pos_t pos=player.play(board);
			history.back().second=pos;
		}
	}
	Score score(board);
	int diff=score.diff();//黑子数减去白子数之差

	uint n=history.size();
	for_n(i, n) {
		Board& b=history[i].first;
		pos_t pos=history[i].second;

		color board_turn=b.get_current_turn();
		int my_diff=(board_turn!=BLACK) ? diff : -diff;

		assert(b.mobility()!=0);
		assert(b.is_active(pos));

		vector<Board> mirror_boards(4, b);//拷贝4份
		mirror_boards[1].mirror_xy();
		mirror_boards[2].mirror_ldru();
		mirror_boards[3].mirror_ldru_xy();

		vector<Move> mirror_moves(4, Move(board_turn, pos));
		mirror_moves[1].mirror_xy();
		mirror_moves[2].mirror_ldru();
		mirror_moves[3].mirror_ldru_xy();

		//添加开局走法
		for_n(j, 4) {
			Board& the_board=mirror_boards[j];
			pos_t the_move=mirror_moves[j].pos;

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
string Game::deal(const string& query) {
	board.init_from_str(query);
	color turn=board.get_current_turn();
	Player& player=(turn==BLACK)?black:white;
	uchar mobility=board.mobility();

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