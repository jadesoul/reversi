#ifndef BOARD_H_
#define BOARD_H_
/*
 * File: board.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年3月12日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "common.h"
#include "valid_move.h"
#include "make_move.h"

//global variables
int turn, oppo;
ulong my, op;
int my_cnt, op_cnt;
int pass_cnt, empty_cnt, played_cnt;

//typedef struct move_t {
//	int 	turn;//who play//no need for use pass
//	int 	pos;//play at where// no need for undo_move(pos)
//	int		win;//how is the play//no need
//	ulong	eat;//flip which stones, used for unplay
//	int		total;//total eat cnt
//} move;

typedef ulong move;

move history[64];//played history, used for unplay
int	sequence[64];//maintain played move pos

//get counters
//#define BLACK_CNT			(turn==BLACK ? my_cnt : op_cnt)
//#define WHITE_CNT			(turn==WHITE ? my_cnt : op_cnt)
//#define STONE_CNT(color)	(color==BLACK ? BLACK_CNT : WHITE_CNT)

//get bits
//#define BLACK_BITS			(turn==BLACK ? my : op)
//#define WHITE_BITS			(turn==WHITE ? my : op)

//swap turn
#define swap_turn(...)		SWAP(turn, oppo); SWAP64(my, op);

//test color at pos
#define is_my(pos)			(BIT_EXIST(my, pos))
#define is_op(pos)			(BIT_EXIST(op, pos))
//#define is_black(pos)		(BIT_EXIST(BLACK_BITS, pos))
//#define is_white(pos)		(BIT_EXIST(WHITE_BITS, pos))
#define is_stone(pos)		(is_black(pos) || is_white(pos))
#define is_empty(pos)		(IS_EMPTY(my, op, pos))

//get color at pos
#define get_color(pos)		(is_my(pos) ? turn : (is_op(pos) ? oppo : EMPTY))

//set color at pos
#define set_my(pos)			{ SET_BIT(my, pos); CLEAR_BIT(op, pos); }
#define set_op(pos)			{ SET_BIT(op, pos); CLEAR_BIT(my, pos); }
//#define set_black(pos)		{ SET_BIT(BLACK_BITS, pos); CLEAR_BIT(WHITE_BITS, pos); }
//#define set_white(pos)		{ CLEAR_BIT(BLACK_BITS, pos); SET_BIT(WHITE_BITS, pos); }
#define set_empty(pos)		{ CLEAR_BIT(my, pos); CLEAR_BIT(op, pos); }

//flip color at pos
//#define flip(pos)			{ if (is_stone(pos)) { FLIP_BIT(my, pos); FLIP_BIT(op, pos); } }

//test if game is over
#define game_over(...)		(empty_cnt==0 || pass_cnt>=2)

//get mobility
#define get_my_mobility(mobility) 	{for (int pos=A1; pos<=H8; ++pos) if (my_valid_move(pos)) ++mobility;}
#define get_op_mobility(mobility) 	{for (int pos=A1; pos<=H8; ++pos) if (op_valid_move(pos)) ++mobility;}
#define get_mobility(mobility)		get_my_mobility(mobility)

#define pass_move(...) swap_turn()
#define unpass_move(...) swap_turn()

#define diff_cnt(...)		(non_iterative_popcount_64(my)-non_iterative_popcount_64(op))
//#define diff_cnt(...)		(my - op)

//return the exact score
//#define get_exact() 		(my_cnt - op_cnt)
#define get_exact() 		diff_cnt()
//#define get_exact() 		0

//#define evaluation()		(my_cnt - op_cnt)
#define evaluation() 		diff_cnt()
//#define evaluation() 		0

void init_board() {
	my=OPEN_BLACK;
	op=OPEN_WHITE;

	my_cnt=2;
	op_cnt=2;
	empty_cnt=60;
	pass_cnt=0;

	turn=BLACK;
	oppo=WHITE;

	played_cnt=4;
}

//for output
void print_board() {
	int pointer = played_cnt -1;
	int last_pos=PASS+1;
	if (pointer>=4) last_pos=sequence[pointer];

	char h = (turn == BLACK ? 'A' : 'a');
	printf("+ ");
	for_n(j, 8) printf("%c ", (char)(h + j));
	printf("+ \n");
	for_n(i, 8)
	{
		printf("%d ", i+1);
		for_n(j, 8)
		{
			int p=POS(i, j);
			int c=get_color(p);
			if (c == EMPTY)
				printf("\033[0;35m.\033[0m");
			else if (c == BLACK)
				printf("\033[0;31mX\033[0m");
			else if (c == WHITE)
				printf("\033[0;33mO\033[0m");
			else
				printf("*");

			int p_east=TO_EAST(p);
			if (last_pos==p) printf("\033[0;34m]\033[0m");
			else if (j<7 && last_pos==p_east) printf("\033[0;36m[\033[0m");
			else printf(" ");
		}
		printf("%d \n", i+1);
	}
	printf("+ ");
	for_n(j, 8) printf("%c ", (char)(h + j));
	printf("+ \n");

	my_cnt=non_iterative_popcount_64(my);
	op_cnt=non_iterative_popcount_64(op);
	uint black_cnt= turn==BLACK ? my_cnt : op_cnt;
	uint white_cnt= turn==WHITE ? my_cnt : op_cnt;
	printf("black=%d white=%d empty=%d pass=%d turn=%s \n", black_cnt, white_cnt, empty_cnt, pass_cnt, COLOR(turn));

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
	printf("--------------------------------------------\n");
}

//if we can make move, return true
int make_move(int pos) {
	if (! is_empty(pos)) return 0;

	assert(is_empty(pos));
	assert(played_cnt<64);

	ulong flip=try_make_move[pos](my, op);
	if (flip==0) return 0;

	//backup last my/op and last eat_mask
//	move m = ;
//	move.turn = turn;
//	move.pos = pos;
//	move.win = win;
//	m.eat = eat;
//	move.eat = val.total_eat;
	history[played_cnt]=flip;
	sequence[played_cnt]=pos;

	//play move
	SET_BIT(my, pos);
	op ^= flip;
	my ^= flip;

	//update counters
//	my_cnt += count1(flip) + 1;
//	op_cnt -= count1(flip);
	--empty_cnt;
//	pass_cnt = 0;
	++played_cnt;//increment index in history

	swap_turn();

	return 1;
}

//recover from last
int undo_move(int pos) {
	assert(played_cnt>4);

	swap_turn();

	--played_cnt;//decrement index in history

	++empty_cnt;

//	move* last = &history[played_cnt];//get last move
	ulong flip=history[played_cnt];
//	turn = last->turn;
//	oppo = OPPO(turn);
//	int pos = last->pos;
//	win = last->win;
//	ulong eat_mask = last->flip;

	//play move
	CLEAR_BIT(my, pos);
	my ^= flip;
	op ^= flip;

	//update counters
//	total[turn] -= last.eat + 1;
//	total[oppo] += last.eat;

//	pass_cnt = 0;//TODO, should this be stored in Move

	return 1;
}

int start_game() {
	while (! game_over()) {
		print_board();
		getchar();
		for (int pos=A1; pos<=H8; ++pos) {
			printf("pos=%d empty=%d \n", pos, empty_cnt);
			if (make_move(pos)) {
				printf("make move from %c%c, played=%d, empty=%d turn=%s \n", TEXT(pos), played_cnt, empty_cnt, COLOR(turn));
				break;
			}
		}
	}
	print_board();
	int win=evaluation();
	return turn==BLACK ? win : -win;
}

//从包含65字节的字符串初始化, 棋盘(64字节)，下子方(1字节)
//为游戏引擎提供此接口
/*
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
*/


#endif /* BOARD_H_ */
