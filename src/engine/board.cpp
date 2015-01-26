/**
 * File: board.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-17 16:27:43.514295
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "board.h"

Board::Board() {
	init_board_map();
}

Board::Board(const string& board) {
	init_from_str(board);
}

Board::~Board() {

}

bool Board::operator <(const Board& another) const {
	const Board* a = this;
	const Board* b = &another;
	if (a == b)
		return true;
	int diff = memcmp(a, b, sizeof(Board));
	return diff < 0;
}

void Board::mirror_xy() {
	for_n(x, 8)
	{
		for_n(y, 8)
		{
			if (x >= y)
				continue;
			pos_t pos = POS(x, y);
			swap(data[pos], data[MIRROR_XY(pos)]);
		}
	}
}

void Board::mirror_ldru() {
	for_n(x, 8)
	{
		for_n(y, 8)
		{
			if (x + y >= 7)
				continue;
			pos_t pos = POS(x, y);
			swap(data[pos], data[MIRROR_IJ(pos)]);
		}
	}
}

void Board::mirror_ldru_xy() {
	mirror_ldru();
	mirror_xy();
}

void Board::set(uchar pos, color c) {
	assert(c==BLACK or c==WHITE or c==EMPTY);

	color orig = BOARD(pos);
	if (orig == c)
		return;

	total[orig] -= 1;
	if (orig == ACTIVE)
		total[EMPTY] -= 1;

	BOARD(pos)=c;

	total[c] += 1;
	//TODO: history已经乱了
}

void Board::pass() {
	assert(total[ACTIVE]==0);
	color oppo = OPPO(get_current_turn());	//交换下子方
	update_possible_moves(oppo);
	++total[PASS];//累加PASS次数
}

pos_t Board::get_first_move() const {
	for (pos_t pos = FIRST; pos < LAST; ++pos) {
		if (BOARD(pos)==ACTIVE) return pos;
	}
	return PASS;
}

size_t Board::play(pos_t pos) {
	uint i = I(pos), j = J(pos);
	color s = turn();	//自己的颜色

	//落子点必须是ACTIVE状态
	if (BOARD(pos)!=ACTIVE) return 0;

	//如果落子成功，则更新新的对手落子点
	BOARD(pos)=s;	//首先在下子处落子
	total[ACTIVE] -= 1;
	total[EMPTY] -= 1;
	total[s] += 1;

	//同时将各个方向上所吃的子翻转为自己的子
	color o = OPPO(s);	//对手的颜色

	size_t eat = 0;	//记录所有方向上总吃子数
	int p;

#define SCAN_FOR_EAT(DIRECTION, INVERSE_DIRECTION) \
	p=DIRECTION(pos); \
	if (BOARD(p)==o) { /*至少有1个对手的子*/ \
		do { \
			p=DIRECTION(p); \
		} \
		while (BOARD(p)==o); \
		if (BOARD(p)==s) { /*能吃子，反向回溯吃子*/ \
			p=INVERSE_DIRECTION(p); \
			do { \
				BOARD(p)=s; /*吃子*/ \
				++eat; \
				p=INVERSE_DIRECTION(p); \
			} \
			while (BOARD(p)==o); \
		} \
	}

	//向8对儿方向扫描
	APPLY_8_PAIR_DIRECTIONS(SCAN_FOR_EAT)

#undef SCAN_FOR_EAT

	assert(eat > 0);

	//把对手的子翻转成了自己的子后，更新各自的棋子数
	total[o] -= eat;
	total[s] += eat;

	update_possible_moves(o);

	//清空连续PASS次数
	total[PASS]=0;

	//更新历史
	int pointer = (60 - 1) - total[EMPTY];	//指向历史中的最后一个有效元素
	assert(pointer>=0 and pointer<sizeof(history));
	history[pointer].pos=pos;
	history[pointer].turn=s;

	return eat;
}

void Board::undo() {
	//
}

size_t Board::potential_mobility(color c) {
	//算法1：将所有颜色是c的棋子周围的所有空格或者ACTIVE状态棋子位置放入集合 O(N*8*log(M)),M为目标棋子数
	//算法2：遍历所有空格，如果其周围8个方向有颜色是c的邻居，则加入列表 O(N*8)
#define USE_ALGORITHM_2
#ifdef USE_ALGORITHM_1
	std::set<int> poss;
	for (pos_t pos=FIRST; pos<LAST; ++pos) {
		if (BOARD(pos)==c) {

#define FIND_EMPTY_OR_ACTIVE(DIRECTION) \
			if (IS_EMPTY_OR_ACTIVE( DIRECTION(pos) )) poss.insert(pos);

			APPLY_8_DIRECTIONS(FIND_EMPTY_OR_ACTIVE)

#undef FIND_EMPTY_OR_ACTIVE

		}
	}
	return poss.size();
#else
	size_t cnt = 0;
	for (pos_t pos = FIRST; pos < LAST; ++pos) {
		if (IS_EMPTY_OR_ACTIVE(pos)) {
			if (NEIGHBOR_HAS_COLOR(pos, c)) {
				++cnt;
			}
		}
	}
	return cnt;
#endif
}

color Board::get_current_turn() const {
	int pointer = (60 - 1) - total[EMPTY];	//指向历史中的最后一个有效元素
	color turn;
	if (pointer == -1)
		turn=BLACK;	//开局黑先
	else
		turn=OPPO(history[pointer].turn);	//上一手的对手
	return turn;
}

void Board::init_from_str(const string& query) {
	assert(query.size() == 65);
	total[EMPTY] = 0;
	total[BLACK] = 0;
	total[WHITE] = 0;
	total[PASS] = 0;
	for_n(x, 8)
	{
		for_n(y, 8)
		{
			uint i = x * 8 + y;
			int pos=POS(x, y);
			char c = query[i];
			if (c == '.') {
				BOARD(pos) = EMPTY;
				total[EMPTY] += 1;
			} else if (c == 'X') {	//代表 BLACK
				BOARD(pos) = BLACK;
				total[BLACK] += 1;
			} else if (c == 'O') {	//代表 WHITE
				BOARD(pos) = WHITE;
				total[WHITE] += 1;
			} else
				assert(false);
		}
	}
	color turn = (query[64] == 'X') ? BLACK : WHITE;

	int pointer = (60 - 1) - total[EMPTY];	//指向历史中的最后一个有效元素
	if (pointer >= 0)
		history[pointer].turn = OPPO(turn);	//只能恢复到上一次下子时的颜色

	update_possible_moves(turn);
}

void Board::init_board_map() {
	for_n(pos, MAP_SIZE)
	{
		if (ON_BOARD(pos)) { //在棋盘上
			if (pos == POS(3, 4) or pos == POS(4, 3)) {
				BOARD(pos)=BLACK;
			} else if (pos==POS(3,3) or pos==POS(4,4)) {
				BOARD(pos)=WHITE;
			} else if (pos==POS(3,2) or pos==POS(2,3) or pos==POS(4,5) or pos==POS(5,4)) {
				//黑子先下的几个位置为激活状态
				BOARD(pos)=ACTIVE;
			} else {
				BOARD(pos)=EMPTY;
			}
		} else { //不在棋盘上，填满墙
			BOARD(pos)=WALL;
		}
	}
	total[EMPTY]=60; //全部空格数，包括ACTIVE状态的激活空格数
	total[WHITE]=2;//白子总数
	total[BLACK]=2;//黑子总数
	total[ACTIVE]=4;//激活空格数
	total[PASS] = 0;

	//	memset(blink, END, BOARD_SIZE);
	//	memset(wlink, END, BOARD_SIZE);

	//	wlink[0]=OFFSET(3, 4);
	//	white[OFFSET(3, 4)]=POS(3,4);
	//	wlink[OFFSET(3, 4)]=OFFSET(4, 3);
	//	white[OFFSET(4, 3)]=POS(4,3);
	//	wlink[OFFSET(4, 3)]=END;
}

void Board::clear_active_states() {	//TODO: 将所有激活状态的棋子记录下来
	for (pos_t i = FIRST; i < LAST; ++i)
		if (data[i] == ACTIVE)
			data[i] = EMPTY;
	total[ACTIVE] = 0;
}

void Board::update_possible_moves(color s) {
	clear_active_states();	//先清除状态
	uint active = 0;	//激活状态数=行动力值

	for (pos_t pos = FIRST; pos < LAST; ++pos) {
		color c = BOARD(pos);
		if (c != s)
			continue;	//寻找所有自己的子

		color o = OPPO(s);	//对方棋子颜色
		int p;	//当前位置

#define SCAN_FOR_ACTIVE(DIRECTION) \
		p=DIRECTION(pos); \
		if (BOARD(p)==o) { \
			do { \
				p=DIRECTION(p); \
			} while (BOARD(p)==o); \
			if (BOARD(p)==EMPTY) { \
				BOARD(p)=ACTIVE; \
				++active; \
			} \
		}

		//向8个方向扫描
		APPLY_8_DIRECTIONS(SCAN_FOR_ACTIVE)

#undef SCAN_FOR_ACTIVE
	}
	total[ACTIVE] = active;
}

ostream& Board::dump(ostream& o) const {
	color turn = get_current_turn();
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
			const color& c = BOARD(POS(i, j));
			if (c == EMPTY)
				o << '.';
			else if (c == BLACK)
				o << 'X';
			else if (c == WHITE)
				o << 'O';
			else
				o << '*';
			o << ' ';
		}
		o << (i + 1) << ' ';
		o << endl;
	}
	o << '+' << ' ';
	for_n(j, 8)
		o << (char) (h + j) << ' ';
	o << '+' << ' ' << endl;
	o << "BLACK=" << (uint) total[BLACK] << " WHITE=" << (uint) total[WHITE];
	o << " EMPTY=" << (uint) total[EMPTY] << " ACTIVE=" << (uint) total[ACTIVE];
	o << " PASS=" << (uint) total[PASS] << " TURN=" << COLOR(turn);

	int pointer = (60 - 1) - total[EMPTY];	//指向历史中的最后一个有效元素
	o << " HISTORY=";
	for_n(i, pointer+1) o<<history[i];
	o << endl;

	return o << "--------------------------------------------" << endl;
}


