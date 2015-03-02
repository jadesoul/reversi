#ifndef BITBOARD_BOARD_H_
#define BITBOARD_BOARD_H_
/*
 * File: board.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月25日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "common.h"
#include "valid_move.h"
#include "make_move.h"

/**
black=		01111101110...
white=		00000010001...
mask=		10000000000...

mask-1=		01111111111

---
black=		01111110
white=		00000001
mask=		10000000
mask-1=		01111111

---
black=		01111100
white=		00000011
mask=		10000000
mask-1=		01111111

---
black=		01101100
white=		00000011
mask=		00010000
mask-1=		00001111

 */

uchar reverse_byte(uchar c)
{
	c = ( c & 0x55 ) << 1 | ( c & 0xAA ) >> 1;
	c = ( c & 0x33 ) << 2 | ( c & 0xCC ) >> 2;
	c = ( c & 0x0F ) << 4 | ( c & 0xF0 ) >> 4;
	return c;
}

//判断棋盘上某一行对应的位字节能否构成吃子(下子点右侧)
bool valid_move_byte_right(uchar my_byte, uchar op_byte, uchar pos/*[0, 8)*/) {
#ifdef ALGO1
	uchar pos_mask_byte = 0x01 << pos;

	//没子了
	if (my_byte==0 or op_byte==0) return false;

	//是否合法: 下子不能有交集
	if ((my_byte & op_byte) !=0) return false;

	//要求下子处是空格
	if ((my_byte & pos_mask_byte) != 0) return false;
	if ((op_byte & pos_mask_byte) != 0) return false;

	//右侧能否吃子
	uchar right_enable_mask = pos_mask_byte - 1;
	uchar right_my_byte = my_byte & right_enable_mask;
	int right = fast_log2_u8_table[right_my_byte];
	uchar right_eat_bits = (right_enable_mask >> right) << right;
	bool right_can_eat = (op_byte & right_eat_bits) == right_eat_bits;
	if (right_can_eat) return right_can_eat;

	//反转字节
	my_byte=reverse_byte(my_byte);
	op_byte=reverse_byte(op_byte);
	pos_mask_byte=0x01 << (7-pos);

	//左侧能否吃子
	uchar left_enable_mask = pos_mask_byte - 1;
	uchar left_my_byte = my_byte & left_enable_mask;
	int left = fast_log2_u8_table[left_my_byte];
	uchar left_eat_bits = (left_enable_mask >> left) << left;
	bool left_can_eat = (op_byte & left_eat_bits) == left_eat_bits;
	return left_can_eat;
#else
	return false;
#endif
}

//快速判断一行上是否能吃子
//预先存放256*256*8个bits，指示对应的可能性是否能吃子：共256*256个字节 my_byte * op_byte
static uchar valid_move_byte_table[256*256];

void init_valid_move_byte_table() {
	for_n(my_byte, 256) {
		for_n(op_byte, 256) {
			uint index=(my_byte << 8) + op_byte;
			valid_move_byte_table[index]=0;
			for_n(pos, 8) {
				uchar pos_mask_byte=0x01 << pos;

				if (valid_move_byte_right(my_byte, op_byte, pos)) {
					valid_move_byte_table[index] |= pos_mask_byte;
					log_status(byte_bitset(my_byte)<<" , "<<byte_bitset(op_byte)<<" : "<<byte_bitset(pos_mask_byte)<<" = 1");
				} else {
					log_status(byte_bitset(my_byte)<<" , "<<byte_bitset(op_byte)<<" : "<<byte_bitset(pos_mask_byte)<<" = 0");
				}
			}
			log_status(byte_bitset(my_byte)<<" , "<<byte_bitset(op_byte)<<"\t=\t"<<byte_bitset(valid_move_byte_table[index]));
		}
	}
}

#define VALID_MOVE_BYTE(my_byte, op_byte, pos_mask_byte) \
	(valid_move_byte_table[(my_byte << 8) + op_byte] & pos_mask_byte)

struct Move {
public:
	color 	turn;//who play
	pos_t 	pos;//play at where
	int		win;//how is the play
	mask_t	flip;//flip which stones, used for unplay
	uint	eat;//total eat cnt

	Move(color turn=BLACK, pos_t pos=PASS, int win=0, mask_t flip=0, uint eat=0)
		:turn(turn), pos(pos), win(win), flip(flip), eat(eat) {}
};

class BitBoard {
public:
	BitBoard() { init_bit_board(); }

	//get counters
	inline size_t stone_cnt(color c) const { return total[c]; }
	inline size_t black_cnt() const { return total[BLACK]; }
	inline size_t white_cnt() const { return total[WHITE]; }

	//get bits
	inline ulong get_bits(color c) const { return bits[c]; }
	inline ulong black_bits() const { return bits[BLACK]; }
	inline ulong white_bits() const { return bits[WHITE]; }

	//get turn
	inline color get_turn() { return turn; }
	inline color next_turn() { return OPPO(turn); }
	inline void swap_turn() { turn=OPPO(turn); }

	//test color at pos : [0, 63]
	inline bool is_color(uint pos, color c) const { return BIT_EXIST(bits[c], pos); }
	inline bool is_black(uint pos) const { return BIT_EXIST(bits[BLACK], pos); }
	inline bool is_white(uint pos) const { return BIT_EXIST(bits[WHITE], pos); }
	inline bool is_empty(ulong b, ulong w, uint pos) const { return IS_EMPTY(b, w, pos); }
	inline bool is_empty(uint pos) const { return IS_EMPTY(bits[BLACK], bits[WHITE], pos); }
	inline bool is_stone(uint pos) const { return is_black(pos) or is_white(pos); }
	inline color get_color(uint pos) const { return is_black(pos) ? BLACK : (is_white(pos) ? WHITE : EMPTY); }

	//set color at pos
	inline void set_black(uint pos) { SET_BIT(bits[BLACK], pos); CLEAR_BIT(bits[WHITE], pos); }
	inline void set_white(uint pos) { CLEAR_BIT(bits[BLACK], pos); SET_BIT(bits[WHITE], pos); }
	inline void set_empty(uint pos) { CLEAR_BIT(bits[BLACK], pos); CLEAR_BIT(bits[WHITE], pos); }

	//flip color at pos
	inline void flip(uint pos)
		{ assert(is_stone(pos)); FLIP_BIT(bits[BLACK], pos); FLIP_BIT(bits[WHITE], pos); }

	//test if we can make move at pos
	inline bool valid_move(const pos_t& pos) {
		color oppo = OPPO(turn);
		const ulong& my = bits[turn];
		const ulong& op = bits[oppo];
		if (! is_empty(my, op, pos)) return false;
//		log_status("check_valid_move from "<< TEXT(pos));
//		log_status("my="<< BITS_TEXT(my));
//		log_status("op="<< BITS_TEXT(op));
		return check_valid_move(my, op, pos);
	}

	//test if game is over
	inline bool game_over() { return empty_cnt==0 or pass_cnt>=2; }

	//you can call valid_move before this or not
	//using eat_table to cache the previous eating mask
	//if we can make move, return true
	inline bool make_move(const pos_t& pos) {
		color oppo = OPPO(turn);
		ulong& my = bits[turn];
		ulong& op = bits[oppo];
		uint& my_cnt = total[turn];
		uint& op_cnt = total[oppo];

		if (! is_empty(my, op, pos)) return false;

		eat_val val;

//		if (! move_db.find(my, op, pos, val)) {//if exist move in db
//			if (! try_make_move(my, op, pos, val)) return false;//begin find move
//			move_db.add(my, op, pos, val);//save new move into db
//		}

		if (! try_make_move(my, op, pos, val)) return false;//begin find move

		//backup last my/op and last eat_mask
		Move& move = history[played_cnt];
		move.turn = turn;
		move.pos = pos;
		move.win = win;
		move.flip = val.eat_mask;
		move.eat = val.total_eat;

		//play move
		op ^= val.eat_mask;
		my ^= val.eat_mask;
		SET_BIT(my, pos);

		//update counters
		my_cnt += val.total_eat + 1;
		op_cnt -= val.total_eat;
		--empty_cnt;
		pass_cnt = 0;
		++played_cnt;//increment index in history

		swap_turn();

		return true;
	}

	uint get_mobility() {
		uint mobility=0;
		for (pos_t pos=A1; pos<=H8; ++pos) {
			if (is_empty(pos)) {
				if (valid_move(pos)) {
					++mobility;
					log_status("found valid move from "<< TEXT(pos));
				}
			}
		}
		return mobility;
	}

	int negamax() {
		// 当前最佳估值，预设为负无穷大
		int best_value = INT32_MIN;
		// 尝试每个下棋位置
		for (pos_t pos=A1; pos<=H8; ++pos) {
			// 试着下这步棋，如果棋步合法
			if (make_move(pos)) {
				// 对所形成的局面进行评估
				int value = -evaluation();
				// 恢复原来的局面
				undo_move();
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

	int negamax(int depth, int pass=0) {
		// 当前最佳估值，预设为负无穷大
		int best_value = INT32_MIN;
		// 如果到达预定的搜索深度
		if (depth <= 0) {
			// 直接给出估值
			return evaluation();
		}
		// 尝试每个下棋位置
		for (pos_t pos=A1; pos<=H8; ++pos) {
			// 试着下这步棋，如果棋步合法
			if (make_move(pos)) {
				// 对所形成的局面进行递归搜索
				int value = -negamax(depth-1, 0);
				// 恢复原来的局面
				undo_move();
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

	int alpha_beta(int alpha, int beta, int depth, int pass=0){
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
				undo_move();
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

	int pvs(int alpha, int beta, int depth, int pass=0){
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
						undo_move();
						// 舍弃这步棋
						continue;
					}
					// 如果这步棋引发剪枝
					if (value >= beta) {
						// 恢复原来的局面
						undo_move();
						// 停止对当前局面的搜索，立即返回
						return value;
					}
					// 保存当前结果以备下一步常规搜索使用
					best_value = alpha = value;
				}
				// 进行常规递归搜索
				int value = -pvs(-beta, -alpha, depth-1, 0);
				// 恢复原来的局面
				undo_move();
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
	int deepening(int seconds=60) {
		timer now;
		int value = 0;
		// 初始搜索深度
		int depth = 1;
		do {
			// 进行常规的MTD(f)算法
			value = mtd(value, depth);
			// 增加搜索深度
			++depth;
			log_status("depth="<<depth<<", seconds="<<now.elapsed());
		// 直到时间用完
		} while (now.elapsed() < seconds) ;
		return value;
	}

	inline void pass_move() {
		swap_turn();
	}

	inline void unpass_move() {
		swap_turn();
	}

	bool search() {
		for (pos_t pos=A1; pos<=H8; ++pos) {
			if (is_empty(pos)) {
//				log_status("found empty: "<< TEXT(pos));
				if (make_move(pos)) {
					log_status("make move from "<< TEXT(pos));
					break;
				}
			}
		}
		return true;
	}

	bool rand_play(uint n) {
		for_n(i, n) {
			for (pos_t pos=A1; pos<=H8; ++pos) {
				if (make_move(pos)) {
//					log_status("make move from "<< TEXT(pos));
					if (game_over()) return false;
					break;
				}
			}
		}
		return true;
	}

	//return the exact score
	int get_exact() {
//		log_status("get_exact: "<< int(total[BLACK]-total[WHITE]));
		return total[BLACK]-total[WHITE];
	}

	int evaluation(color c=EMPTY) {
		if (c==EMPTY) c=turn;
		win=total[c]-total[OPPO(c)];
//		log_status("got evaluation: "<< win);
		return win;
	}

	int start_game() {
		while (! game_over()) {
//			log_status((*this)<<move_db);
//			getchar();
			for (pos_t pos=A1; pos<=H8; ++pos) {
				if (is_empty(pos)) {
					if (make_move(pos)) {
						log_status("make move from "<< TEXT(pos));
						break;
					}
				}
			}
		}
		evaluation(BLACK);
//		log_status((*this)<<move_db);
		return win;
	}

	bool undo_move() {
		//recover from last
		if (played_cnt==0) return false;

		--played_cnt;//decrement index in history
		Move& last = history[played_cnt];//get last move
		turn = last.turn;
		pos_t pos = last.pos;
		win = last.win;
		mask_t eat_mask = last.flip;

		//play move
		bits[BLACK] ^= eat_mask;
		bits[WHITE] ^= eat_mask;
		CLEAR_BIT(bits[turn], pos);

		//update counters
		total[turn] -= last.eat + 1;
		total[OPPO(turn)] += last.eat;

		++empty_cnt;
		pass_cnt = 0;//TODO, should this be stored in Move

		return true;
	}

	void init_bit_board() {
		bits[BLACK]=OPEN_BLACK;
		bits[WHITE]=OPEN_WHITE;

		total[BLACK]=2;
		total[WHITE]=2;
		empty_cnt=60;
		pass_cnt=0;

		turn=BLACK;

		played_cnt=0;
		win=0;
	}

	friend inline ostream& operator<<(ostream& o, const BitBoard& b) { b.dump(o); return o; }

	//for output
	void dump(ostream& o) const {
		int pointer = played_cnt -1;
		pos_t last_pos=PASS;
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
				pos_t p=POS(i, j);
				const color& c = get_color(p);
				if (c == EMPTY)
					o << '.';
				else if (c == BLACK)
					o << 'X';
				else if (c == WHITE)
					o << 'O';
				else
					o << '*';

				pos_t p_east=TO_EAST(p);
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

public:
	//board map
	ulong bits[2];

	//counters
	uint total[2];
	uint empty_cnt;
	uint pass_cnt;
	uint played_cnt;//how many moves have been played, pointing to the next index in history

	//for whose turn to play
	color turn;

	//current evaluate score
	int win;

	//play history
	Move history[64];//save each play pos, color, win, flip mask
};

#endif /* BITBOARD_BOARD_H_ */
