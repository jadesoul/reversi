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

// global variables
int turn, oppo;
ulong my, op, em;// myself, opponent, empty
int my_cnt, op_cnt;
int empty_cnt, played_cnt;
int pass_cnt;
ulong search_nodes, search_leafs_eval, search_leafs_exact, search_cut_branchs;// total search board state in alpha_beta, mtd, etc.

//typedef struct move_t {
//	int 	turn;	//who play			//no need for use pass
//	int 	pos;	//play at where		//no need for undo_move(pos)
//	int		win;	//how is the play	//no need
//	ulong	eat;	//flip which stones, used for unplay
//	int		total;	//total eat cnt
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
#define swap_turn(...)		SWAP(turn, oppo); SWAP64(my, op) // SWAP(my_cnt, op_cnt);

//test color at pos
#define is_my(pos)			(BIT_EXIST(my, pos))
#define is_op(pos)			(BIT_EXIST(op, pos))
//#define is_black(pos)		(BIT_EXIST(BLACK_BITS, pos))
//#define is_white(pos)		(BIT_EXIST(WHITE_BITS, pos))
//#define is_stone(pos)		(is_black(pos) || is_white(pos))

//#define is_empty(pos)		(IS_EMPTY(my, op, pos))
#define is_empty(pos)		(BIT_EXIST(em, pos))
#define is_stone(pos)		(BIT_NOT_EXIST(em, pos))

//get color at pos
#define get_color(pos)		(is_my(pos) ? turn : (is_op(pos) ? oppo : EMPTY))

//set color at pos
#define set_my(pos)			{ SET_BIT(my, pos); CLEAR_BIT(op, pos); }// need CLEAR_BIT(em, pos);
#define set_op(pos)			{ SET_BIT(op, pos); CLEAR_BIT(my, pos); }// need CLEAR_BIT(em, pos);
//#define set_black(pos)		{ SET_BIT(BLACK_BITS, pos); CLEAR_BIT(WHITE_BITS, pos); }
//#define set_white(pos)		{ CLEAR_BIT(BLACK_BITS, pos); SET_BIT(WHITE_BITS, pos); }
#define set_empty(pos)		{ CLEAR_BIT(my, pos); CLEAR_BIT(op, pos); }//SET_BIT(em, pos);

//flip color at pos
//#define flip(pos)			{ if (is_stone(pos)) { FLIP_BIT(my, pos); FLIP_BIT(op, pos); } }

//test if game is over
#define game_over(...)		(empty_cnt==0 || pass_cnt>=2)

//get mobility
#define get_my_mobility(mobility) 	{mobility=0; for (int pos=A1; pos<=H8; ++pos) if (my_valid_move(pos)) ++mobility;}
#define get_op_mobility(mobility) 	{mobility=0; for (int pos=A1; pos<=H8; ++pos) if (op_valid_move(pos)) ++mobility;}
#define get_mobility(mobility)		get_my_mobility(mobility) // default as get my mobility

#define pass_move(...) { swap_turn(); pass_cnt++; } // need pass_cnt++
#define unpass_move(...) { swap_turn(); pass_cnt--; } // need pass_cnt--

#define diff_cnt(...)		(non_iterative_popcount_64(my) - non_iterative_popcount_64(op))
//#define diff_cnt(...)		(my - op)

//return the exact score
//#define get_exact() 		(my_cnt - op_cnt)
// #define get_exact() 		( (op==0) ? 65 : diff_cnt() ) // why 65 ?
// #define get_exact() 		( (op==0) ? non_iterative_popcount_64(my) : diff_cnt() )
//#define get_exact() 		0
#define get_exact() 		diff_cnt()

int get_stable(ulong my, ulong op) { // TODO
//	ulong e, se, s, sw, w, nw, n, ne;//in this direction, is the stone in pos linked with edge
//	int cnt;
//	for (int pos=A1; pos<=H8; ++pos) {
//		int i=I(pos), j=J(pos);
//		ulong m=ONE << pos;
//		if (e & m || w & m) ++cnt;
//		else if (s & m || n & m) ++cnt;
//		else if (s & m || n & m) ++cnt;
//		else if (s & m || n & m) ++cnt;
//	}
	return 0;
}

void print_board();

int evaluation() {
	// print_board();
	// getchar();
	// return 0;

	if (op==0) return non_iterative_popcount_64(my);
	if (my==0) return -non_iterative_popcount_64(op);
	if (em==0) return non_iterative_popcount_64(my) - non_iterative_popcount_64(op);
	if (played_cnt<=20) {	//opening
		int m1, m2;
		get_my_mobility(m1);
		get_op_mobility(m2);
		return m1-m2;
	} else if (played_cnt<=40) {	//middle game 估值很不准
//		return get_stable(my, op)-get_stable(op, my);
//		int a=non_iterative_popcount_32((my >> 56) | ((my & 0xFF) << 8));
//		int b=non_iterative_popcount_32((op >> 56) | ((op & 0xFF) << 8));
//		return a-b;
		// int corner = non_iterative_popcount_64(my & 0x8100000000000081UL)-non_iterative_popcount_64(op & 0x8100000000000081UL);
		// return corner;
		int boder_corner = non_iterative_popcount_64(my & 0xFF818181818181FFUL)-non_iterative_popcount_64(op & 0xFF818181818181FFUL);
		return boder_corner;
	} else if (played_cnt<=50) { // end game
		int boder_corner = non_iterative_popcount_64(my & 0xFF818181818181FFUL)-non_iterative_popcount_64(op & 0xFF818181818181FFUL);
		return boder_corner;
	} else { // 60 - 64
		return non_iterative_popcount_64(my) - non_iterative_popcount_64(op);
	}
}

void init_board() {
	my=OPEN_BLACK; // 黑棋
	op=OPEN_WHITE; // 白棋
	em=ALLONE ^ (OPEN_BLACK | OPEN_WHITE); // 空格

	my_cnt=2;
	op_cnt=2;

	played_cnt = my_cnt + op_cnt; // 4

	empty_cnt= LEN * LEN - played_cnt; // 64 - 4 = 60
	pass_cnt=0;

	turn=BLACK; // 黑先
	oppo=WHITE;
}

//for output
void print_board() {
	int pointer = played_cnt - 1;
	int last_pos = PASS + 1;
	if (pointer>=4) last_pos = sequence[pointer];

	int mobility;
	get_mobility(mobility);

	printf("--------------------------------------------\n");
	char h = (turn == BLACK ? 'A' : 'a');
	// printf("+ ");
	printf("  ");
	for_n(j, 8) printf("%c ", (char)(h + j));
	// printf("+ \n");
	printf("  \n");
	for_n(i, 8)
	{
		printf("%d ", i+1);
		for_n(j, 8)
		{
			int p=POS(i, j);

			int c=get_color(p);
			if (c == EMPTY)
				// if (valid_move(p)) printf(turn==BLACK ? "*" : "*");
				// if (valid_move(p)) printf(turn==BLACK ? "\033[0;32m.\033[0m" : ".");
				// if (valid_move(p)) printf("\033[0;32m.\033[0m");
				if (valid_move(p)) printf(".");

				// else printf("\033[0;35m.\033[0m");
				else printf(" ");
			else if (c == BLACK)
				// printf("\033[0;31m▪\033[0m");
				// printf("\033[0;31m●\033[0m");
				printf("●");
			else if (c == WHITE)
				// printf("\033[0;33mO\033[0m");
				// printf("\033[0;33m○\033[0m");
				printf("○");
			else
				printf("*");

			int p_east=TO_EAST(p);
			if (last_pos==p) printf("\033[0;36m]\033[0m");
			else if (j<7 && last_pos==p_east) printf("\033[0;36m[\033[0m");
			else printf(" ");
		}
		printf("%d \n", i+1);
	}
	// printf("+ ");
	printf("  ");
	h='1';
	for_n(j, 8) printf("%c ", (char)(h + j));
	// printf("+ \n");
	printf("  \n");

	my_cnt=non_iterative_popcount_64(my);
	op_cnt=non_iterative_popcount_64(op);
	uint black_cnt= turn==BLACK ? my_cnt : op_cnt;
	uint white_cnt= turn==WHITE ? my_cnt : op_cnt;
	printf("black=%d white=%d played=%d empty=%d mobility=%d pass_cnt=%d turn=%s \n", black_cnt, white_cnt, played_cnt, empty_cnt, mobility, pass_cnt, COLOR(turn));

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
	
}

//if we can make move, return true
int make_move(int pos) {
	if (! is_empty(pos)) return 0;

	assert(is_empty(pos));
	assert(played_cnt<64);

	ulong flip=try_make_move[pos](my, op);
	if (flip==0) return 0;

	//backup last my/op and last make move color ?
	history[played_cnt]=flip;
	sequence[played_cnt]=pos;

	//play move
	ulong pm=1ul << pos;
	my ^= flip | pm;
	op ^= flip;
	em ^= pm;

	--empty_cnt;
	
	//	pass_cnt = 0;
	++played_cnt;//increment index in history

	swap_turn();
	pass_cnt = 0; //made a new move, reset pass_cnt

	return 1;
}

//recover from last
int undo_move(int pos) {
	assert(played_cnt>4);

	swap_turn();

	--played_cnt;//decrement index in history

	++empty_cnt;

	//get last move
	ulong flip=history[played_cnt];

	//play move
	ulong pm=1ul << pos;
	my ^= flip | pm;
	op ^= flip;
	em ^= pm;

//	pass_cnt = 0;//TODO, should this be stored in Move

	return 1;
}

// Function to shuffle an array using Fisher-Yates algorithm
void shuffle(int *array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[i];
            array[i] = array[j];
            array[j] = t;
        }
    }
}

// TODO: found a bug: if one side has no mobility, should pass, and change turn
// currently , the program will loop forever
// FIXED: 2025.03.01 already fix this bug by pass_cnt
int start_game(int verbose, int randplay) {
	// init a pos array
	// int pos_array[SIZE];
	// for (int pos=A1; pos<=H8; ++pos) {
	// 	pos_array[pos]=pos;
	// }

	while (! game_over()) {
		if (verbose) print_board();
		// getchar();
		int valid_cnt = 0;
		// if (randplay) { // shuffle the pos array
			// shuffle(pos_array, SIZE); // too slow
		// }
		for (int pos_idx=A1; pos_idx<=H8*3; ++pos_idx) {//try 3x64 times rand play
			// int pos = pos_array[pos_idx];
			// int pos = pos_idx;
			uint pos = randplay ? rand() : pos_idx;
			pos = pos % SIZE;
			// int pos = randplay? rand() % SIZE : pos_idx % SIZE;
//			printf("pos=%d empty=%d \n", pos, empty_cnt);
			if (valid_move(pos)) {
				valid_cnt++;
//				printf("make move from %c%c, played=%d, empty=%d turn=%s \n", TEXT(pos), played_cnt, empty_cnt, COLOR(turn));
				if (verbose) printf("%s is going to make move on %c%c\n", COLOR(turn), TEXT(pos));
				if (make_move(pos)) {
					if (verbose) {
						getchar();
					}
					break;
				}
			}
		}
		if (valid_cnt == 0) { // no valid move
			//pass once
			if (verbose) {
				printf("no valid move, %s pass\n", COLOR(turn));
				getchar();
			}
			pass_move();
		}
	}
	// if (verbose || played_cnt<64) print_board();
	int win=evaluation();
	if (verbose) printf("win=%d \n", turn==BLACK ? win : -win);
	return turn==BLACK ? win : -win;
}

//TODO: still has bug, pass cnt == 2 , but white can play
// + a b c d e f g h +
// 1 O O O O O O O O 1
// 2 O O O O O O O O 2
// 3 O O X X X X O O 3
// 4 O O X O X O O O 4
// 5 O O X O X X O O 5
// 6 O O X O X O X O 6
// 7 O O X X O X O O 7
// 8 O *[X]O O O O O 8
// + 1 2 3 4 5 6 7 8 +
// black=16 white=47 played=63 empty=1 mobility=1 pass_cnt=2 turn=WHITE


#endif /* BOARD_H_ */
