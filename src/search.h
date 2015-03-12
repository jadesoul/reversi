#ifndef SEARCH_H_
#define SEARCH_H_
/*
 * File: search.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年3月12日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "board.h"

int negamax_raw() {
	// 当前最佳估值，预设为负无穷大
	int best_value = INT32_MIN;
	// 尝试每个下棋位置
	for (int pos=A1; pos<=H8; ++pos) {
		// 试着下这步棋，如果棋步合法
		if (make_move(pos)) {
			// 对所形成的局面进行评估
			int value = -evaluation();
			// 恢复原来的局面
			undo_move(pos);
			// 如果这步棋更好
			if (value > best_value) {
				// 保存更好的结果
				best_value = value;
			}
		}
	}
	// 如果没有合法棋步
	if (best_value == INT32_MIN) {//PASS
		// 直接评估当前局面
		best_value = evaluation();
	}
	// 返回最佳估值
	return best_value;
}

int negamax(int depth, int pass) {
	// 当前最佳估值，预设为负无穷大
	int best_value = INT32_MIN;
	// 如果到达预定的搜索深度
	if (depth <= 0) {
		// 直接给出估值
		return evaluation();
	}
	// 尝试每个下棋位置
	for (int pos=A1; pos<=H8; ++pos) {
		// 试着下这步棋，如果棋步合法
		if (make_move(pos)) {
			// 对所形成的局面进行递归搜索
			int value = -negamax(depth-1, 0);
			// 恢复原来的局面
			undo_move(pos);
			// 如果这步棋更好
			if (value > best_value) {
				// 保存更好的结果
				best_value = value;
			}
		}
	}
	// 如果没有合法棋步
	if (best_value == INT32_MIN) {
		// 如果上一步棋也是弃着，表明对局结束
		if (pass) {
			// 计算出盘面的精确比分
			return get_exact();
		}
		// 否则这步棋弃着
		pass_move();
		// 递归搜索，并标明该步弃着
		best_value = - negamax(depth, 1);
		// 恢复原来的局面
		unpass_move();
	}
	// 返回最佳估值
//		log_status("best="<<best_value<<", depth="<<depth);
	return best_value;
}

int alpha_beta(int alpha, int beta, int depth, int pass){
	// 当前最佳估值，预设为负无穷大
	int best_value = INT32_MIN;
	// 如果到达预定的搜索深度
	if (depth <= 0) {
		// 直接给出估值
		return evaluation();
	}
	// 尝试每个下棋位置
	for (int pos=A1; pos<=H8; ++pos) {
		// 试着下这步棋，如果棋步合法
		if (make_move(pos)) {
			// 对所形成的局面进行递归搜索
			int value = -alpha_beta(-beta, -alpha, depth-1, 0);
			// 恢复原来的局面
			undo_move(pos);
			// 如果这步棋引发剪枝
			if (value >= beta) {
				// 停止对当前局面的搜索，立即返回
				return value;
			}
			// 如果这步棋更好
			if (value > best_value) {
				// 保存更好的结果
				best_value = value;
				// 更新估值下限
				if (value > alpha) {
					alpha = value;
				}
			}
		}
	}
	// 如果没有合法棋步
	if (best_value == INT32_MIN) {
		// 如果上一步棋也是弃着，表明对局结束
		if (pass) {
			// 计算出盘面的精确比分
			return get_exact();
		}
		// 否则这步棋弃着
		pass_move();
		// 递归搜索，并标明该步弃着
		best_value = -alpha_beta(-beta, -alpha, depth, 1);
		// 恢复原来的局面
		unpass_move();
	}
	// 返回最佳估值
	return best_value;
}

int pvs(int alpha, int beta, int depth, int pass){
	// 当前最佳估值，预设为负无穷大
	int best_value = INT32_MIN;
	// 如果到达预定的搜索深度
	if (depth <= 0) {
		// 直接给出估值
		return evaluation();
	}
	// 尝试每个下棋位置
	for (int pos=A1; pos<=H8; ++pos) {
		// 试着下这步棋，如果棋步合法
		if (make_move(pos)) {
			// 如果之前已经找到一个PV节点
			if (best_value == alpha) {
				// 先进行零宽窗口搜索
				int value = -pvs(-alpha-1, -alpha, depth-1, 0);
				// 如果不可能存在更好的估值
				if (value <= alpha) {
					// 恢复原来的局面
					undo_move(pos);
					// 舍弃这步棋
					continue;
				}
				// 如果这步棋引发剪枝
				if (value >= beta) {
					// 恢复原来的局面
					undo_move(pos);
					// 停止对当前局面的搜索，立即返回
					return value;
				}
				// 保存当前结果以备下一步常规搜索使用
				best_value = alpha = value;
			}
			// 进行常规递归搜索
			int value = -pvs(-beta, -alpha, depth-1, 0);
			// 恢复原来的局面
			undo_move(pos);
			// 如果这步棋引发剪枝
			if (value >= beta) {
				// 停止对当前局面的搜索，立即返回
				return value;
			}
			// 如果这步棋更好
			if (value > best_value) {
				// 保存更好的结果
				best_value = value;
				// 更新估值下限
				if (value > alpha) {
					alpha = value;
				}
			}
		}
	}
	// 如果没有合法棋步
	if (best_value == INT32_MIN) {
		// 如果上一步也是弃着，表明对局结束
		if (pass) {
			// 计算出盘面的精确比分
			return get_exact();
		}
		// 否则这步棋弃着
		pass_move();
		// 递归搜索，并标明该步弃着
		best_value = -pvs(-beta, -alpha, depth, 1);
		// 恢复原来的局面
		unpass_move();
	}
	// 返回最佳估值
	return best_value;
}

int mtd(int f, int depth){
	int g = f;
	int beta;
	// 搜索区间上、下限
	int lower = INT32_MIN;
	int upper = INT32_MAX;
	do {
		// 确定试探值
		if (g == lower) {
			beta = g + 1;
		} else {
			beta = g;
		}
		// 进行零宽窗口试探
		g = alpha_beta(beta-1, beta, depth, 0);
		// 调整搜索区间
		if (g < beta) {
			upper = g;
		} else {
			lower = g;
		}
	} while (lower < upper);
	// 如果最后一次搜索得到的只是上限，需再搜索一次，确保获得正确的最佳棋步
	if (g < beta) {
		g = alpha_beta(g-1, g, depth, 0);
	}
	return g;
}

//迭代加深搜索
int deepening(int seconds) {
//	TIME_START(now)
	int value = 0;
	// 初始搜索深度
	int depth = 1;
	float elapsed;
	do {
		// 进行常规的MTD(f)算法
		value = mtd(value, depth);
//		log_status("depth="<<depth<<", seconds="<< TIME_ELASPED(now) <<", value="<<value);
		// 增加搜索深度
		++depth;
//		elapsed=TIME_ELASPED(now);
	// 直到时间用完
	} while (/*elapsed < seconds and */depth<=empty_cnt) ;
	return value;
}

inline void pass_move() {
	swap_turn();
}

inline void unpass_move() {
	swap_turn();
}

int search() {
	for (int pos=A1; pos<=H8; ++pos) {
		if (is_empty(pos)) {
//				log_status("found empty: "<< TEXT(pos));
			if (make_move(pos)) {
//				log_status("make move from "<< TEXT(pos));
				break;
			}
		}
	}
	return 1;
}

int rand_play(uint n) {
	for_n(i, n) {
		for (int pos=A1; pos<=H8; ++pos) {
			if (make_move(pos)) {
//					log_status("make move from "<< TEXT(pos));
				if (game_over()) return 0;
				break;
			}
		}
	}
	return 1;
}

int start_game() {
	while (! game_over()) {
//			log_status((*this)<<move_db);
//			getchar();
		for (int pos=A1; pos<=H8; ++pos) {
			if (is_empty(pos)) {
				if (make_move(pos)) {
//					log_status("make move from "<< TEXT(pos));
					break;
				}
			}
		}
	}
	int win=evaluation();
//		log_status((*this)<<move_db);
	return turn==BLACK ? win : -win;
}

void init_bit_board() {
	my=OPEN_BLACK;
	op=OPEN_WHITE;

	my_cnt=2;
	op_cnt=2;
	empty_cnt=60;
	pass_cnt=0;

	turn=BLACK;
	oppo=WHITE;

	played_cnt=4;

	for_n(i, 64) sequence[i]=i;
	sequence[D4]=0;
	sequence[D5]=1;
	sequence[E4]=2;
	sequence[E5]=3;
	sequence[0]=D4;
	sequence[1]=D5;
	sequence[2]=E4;
	sequence[3]=E5;
}

//从包含65字节的字符串初始化, 棋盘(64字节)，下子方(1字节)
//为游戏引擎提供此接口
void init_from_str(const char* query) {
	assert(strlen(query) == 65);
	my=0;
	op=0;
	empty_cnt = 0;
	my_cnt = 0;
	op_cnt = 0;
	pass_cnt = 0;

	//assume the turn is BLACK
	for_n(x, 8)
	{
		for_n(y, 8)
		{
			int pos=POS(x, y);
			char c = query[pos];
			if (c == '0') {
				set_empty(pos);
				empty_cnt += 1;
			} else if (c == '1') {	// BLACK
				set_black(pos);
				my_cnt += 1;
			} else if (c == '2') {	// WHITE
				set_white(pos);
				op_cnt += 1;
			} else
				assert(0);
		}
	}
	played_cnt = 60-empty_cnt;
	turn = (query[64] == '1') ? BLACK : WHITE;

	//otherwise, swap
	if (turn==WHITE) {
		SWAP64(my, op);
		SWAP(my_cnt, op_cnt);
		oppo = OPPO(turn);
	}

	//change turn for oppo play
	oppo = OPPO(turn);
}

//用于游戏引擎，给定字符串（64字符的游戏局面和1个字符的turn）
//返回下子的位置坐标 (2个字符)，下标均是从1开始计算
const char* deal(const char* query) {
	init_from_str(query);

//		int mob = get_mobility();
//		if (mob==0) return "00";//表示PASS
//
//		int avg_think_time = 3.0 / mob;

	int depth = empty_cnt<=18 ? empty_cnt : 10;
	// 当前最佳估值，预设为负无穷大
	int best_value = INT32_MIN;
	int best_pos = PASS;
	// 尝试每个下棋位置

	for (int pos=A1; pos<=H8; ++pos) {
		// 试着下这步棋，如果棋步合法
		if (make_move(pos)) {
//				if (depth==10 and rand()%7==1) {
//					best_pos= pos;
//					break;
//				}
			// 对所形成的局面进行评估

//				int value = -deepening(avg_think_time);
//				int value = - mtd(0, depth-1);
			int value = - pvs(-64, 64, depth-1);

			// 恢复原来的局面
			undo_move(pos);
			// 如果这步棋更好
			if (value > best_value) {
				// 保存更好的结果
				best_value = value;
				best_pos = pos;
			}
		}
	}

	if (best_pos==PASS) {
		return "00";//表示PASS
	}

	int x=I(best_pos), y=J(best_pos);
	char s[3];
	s[0]='1'+x;//从1开始编号
	s[1]='1'+y;
	s[2]=0;

//	log_status("played="<<played_cnt<<", empty="<<empty_cnt<<", depth="<<depth<<", pos="<<TEXT(best_pos) <<", win="<<(turn==BLACK?best_value:-best_value)<<", turn="<<COLOR(turn));
	return s;
}

/*
//for output
void dump(ostream& o) const {
	int pointer = played_cnt -1;
	int last_pos=PASS;
	if (pointer>=0) last_pos=history[pointer].pos;

	char h = (turn == BLACK ? 'A' : 'a');
	o << endl << '+' << ' ';
	for_n(j, 8)
		o << (char) (h + j) << ' ';
	o << '+' << ' ' << endl;
	for_n(i, 8)
	{
		o << (i + 1) << ' ';
		for_n(j, 8)
		{
			int p=POS(i, j);
			const color& c = get_color(p);
			if (c == EMPTY)
				o << '.';
			else if (c == BLACK)
				o << 'X';
			else if (c == WHITE)
				o << 'O';
			else
				o << '*';

			int p_east=TO_EAST(p);
			if (last_pos==p) o<< ']';
			else if (last_pos==p_east) o<<'[';
			else o << ' ';
		}
		o << (i + 1) << ' ';
		o << endl;
	}
	o << '+' << ' ';
	for_n(j, 8)
		o << (char) (h + j) << ' ';
	o << '+' << ' ' << endl;
	o << "black=" << (uint) total[BLACK] << " white=" << (uint) total[WHITE];
	o << " empty=" << empty_cnt << " active=" << 0;
	o << " pass=" << pass_cnt << " turn=" << COLOR(turn) << " win="<<win;
//		o << " hash=0x"<< std::hex << hash << std::dec;
	o << endl;

//		o << "black_stable=" << get_stable_stones_size(BLACK);
//		o << " white_stable=" << get_stable_stones_size(WHITE);
//		o << " eval=" << evaluate_and_predict_win_score();
//		o << endl;

//	o << "history=";
//	for_n(i, pointer+1) o<<history[i]<<' ';

//		o << "history="<<endl;
//		for_n(i, pointer+1) {
//			o<< i+1 <<":\t";
//			if (history[i].turn==WHITE) o<<"\t\t";
//			o<<history[i];
//			o<<endl;
//		}
//
//		o << endl;

	o << "--------------------------------------------" << endl;
}
*/


#endif /* SEARCH_H_ */
