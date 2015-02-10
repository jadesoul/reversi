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
	init_board_hash();
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
	init_board_hash();
}

void Board::mirror_ldru_xy() {
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

	init_board_hash();
}

void Board::set(pos_t pos, color c) {
	assert(c==BLACK or c==WHITE or c==EMPTY);

	color orig = BOARD(pos);
	if (orig == c)
		return;

	total[orig] -= 1;
	if (orig == ACTIVE) {
		total[EMPTY] -= 1;
		hash ^= board_hash_table[pos][EMPTY];//先还原以前的hash
	} else {
		hash ^= board_hash_table[pos][orig];//先还原以前的hash
	}

	BOARD(pos)=c;
	hash ^= board_hash_table[pos][c];//再更新当前hash

	total[c] += 1;
}

void Board::pass() {
	assert(total[ACTIVE]==0);
	color s=turn();
	color o = OPPO(s);//交换下子方
	update_possible_moves(o);

	//更新交换下子方后的hash
	hash ^= board_hash_table[MAP_SIZE][s];/*先还原以前的hash*/
	hash ^= board_hash_table[MAP_SIZE][o];/*再更新当前hash*/

	++total[PASS];//累加PASS次数
}

pos_t Board::get_first_move() const {
	for (pos_t pos = FIRST; pos < LAST; ++pos) {
		if (BOARD(pos)==ACTIVE) return pos;
	}
	return PASS;
}

size_t Board::play(pos_t pos) {
//	uint i = I(pos), j = J(pos);
	color s = turn();	//自己的颜色
	assert(s==BLACK or s==WHITE);

	//落子点必须是ACTIVE状态
	if (BOARD(pos)!=ACTIVE) return 0;

	//如果落子成功，则更新新的对手落子点
	hash ^= board_hash_table[pos][EMPTY];/*先还原以前的hash*/
	BOARD(pos)=s;	//首先在下子处落子
	hash ^= board_hash_table[pos][s];/*再更新当前hash*/

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
				hash ^= board_hash_table[p][o];/*先还原以前的hash*/ \
				BOARD(p)=s; /*吃子*/ \
				hash ^= board_hash_table[p][s];/*再更新当前hash*/ \
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

	update_possible_moves(o);//轮到对手下子

	//更新交换下子方后的hash
	hash ^= board_hash_table[MAP_SIZE][s];/*先还原以前的hash*/
	hash ^= board_hash_table[MAP_SIZE][o];/*再更新当前hash*/

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

size_t Board::potential_mobility(color c) const {
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
	if (pointer == -1) {
		turn=BLACK;	//开局黑先
	} else {
//		assert(total[PASS]<2);
		turn=(total[PASS] % 2 == 1) ? history[pointer].turn : OPPO(history[pointer].turn);	//上一手的对手
	}
	return turn;
}

double Board::evaluate_and_predict_win_score() const {
	double m=mobility();
//    return m;
	color s = turn();
	color o = OPPO(s);


	//评估函数策略，开局主要看稳定子，中盘看行动力，收官直接完美搜索
	int e=empty_cnt();
	int d=60-e;

	bool opening2middle=(d<=16);

#define USE_COMPLEX_RULE_OPEN2MID
#ifdef USE_COMPLEX_RULE_OPEN2MID
	//判断从开局进入中盘的标准：第一颗子下在边上
	if (! opening2middle) {
		for_n(i, 8) {
			pos_t p1=POS(i, 0), p2=POS(i, 7);
			color c1=BOARD(p1), c2=BOARD(p2);
			if (IS_STONE(c1) or IS_STONE(c2)) {
				opening2middle=true;
				break;
			}
		}
	}

	if (! opening2middle) {
		for (int j=1; j<7; ++j) {
			pos_t p1=POS(0, j), p2=POS(7, j);
			color c1=BOARD(p1), c2=BOARD(p2);
			if (IS_STONE(c1) or IS_STONE(c2)) {
				opening2middle=true;
				break;
			}
		}
	}

#endif

	if (! opening2middle) {//开局
		int p_s = potential_mobility(s);
		int p_o = potential_mobility(o);
		int win_potential = p_o - p_s;
		return win_potential;
	} else /*if (d<=50)*/ {//中盘//收官
		int s_s = get_stable_stones_size(s);
		int s_o = get_stable_stones_size(o);
		int win_stable= s_s - s_o;

		int good=0;
		if (is_active(LU)) ++good;
		if (is_active(RU)) ++good;
		if (is_active(LD)) ++good;
		if (is_active(RD)) ++good;

//		if (IS_COLOR(LU1, s)) --good;
//		if (IS_COLOR(LU2, s)) --good;
//		if (IS_COLOR(LU3, s)) --good;
//
//		if (IS_COLOR(RU1, s)) --good;
//		if (IS_COLOR(RU2, s)) --good;
//		if (IS_COLOR(RU3, s)) --good;
//
//		if (IS_COLOR(LD1, s)) --good;
//		if (IS_COLOR(LD2, s)) --good;
//		if (IS_COLOR(LD3, s)) --good;
//
//		if (IS_COLOR(RD1, s)) --good;
//		if (IS_COLOR(RD2, s)) --good;
//		if (IS_COLOR(RD3, s)) --good;

		return win_stable+2*good;

//		if (win_stable>0) return win_stable;
//		else return m+win_stable;
	}
	return 0;
}

size_t Board::get_stable_stones_size(color s) const {

//#define GET_STABLE_ALGO_1

#ifdef GET_STABLE_ALGO_1
	//基本思想：从4个角出发，寻找和角相连的边上的子
	size_t cnt=0;
	pos_t p;

#define SCAN_FOR_STABLE(VERTEX, DERECTION1, DERECTION2) \
	if (IS_COLOR(VERTEX, s)) { \
		p=VERTEX; \
		do { \
			++cnt; \
			p=DERECTION1(p); \
		} while (IS_COLOR(p, s)); \
		p=VERTEX; \
		do { \
			++cnt; \
			p=DERECTION2(p); \
		} while (IS_COLOR(p, s)); \
	}

	APPLY_4_TUPLE_DIRECTIONS(SCAN_FOR_STABLE)

#undef SCAN_FOR_STABLE

	return cnt;
#else

	//判断是稳定子的标准：
	//	从当前子出发，如果横竖撇捺四条线（8个方向），每条线上都至少有一个方向满足：
	//	当前子直接与墙相邻，或者当前子通过连续的自己的子与墙相连
	//	如果某一条线的两个方向都满足当前子两侧都没有空格，也可以

	uchar touching_wall[MAP_SIZE];//每个棋子在哪些方向上直接或间接地挨着墙壁
//	uchar has_empty[MAP_SIZE];//每个棋子的各个方向的一侧是否有空格
	memset(touching_wall, 0, sizeof(uchar)*MAP_SIZE);
//	memset(has_empty, 0, sizeof(uchar)*MAP_SIZE);
	size_t cnt=0;
	pos_t p;

	//射线法，从边缘的墙出发，向各个方向投射射线
	for_n(pos, MAP_SIZE) {
		if (BOARD(pos)==WALL) {

#define SCAN_FOR_STABLE_BY_LIGHTING(DIRECTION, ENUM) \
			p=DIRECTION(pos); \
			if (p<MAP_SIZE and (BOARD(p)==s)) { \
				do { \
					touching_wall[p] |= ENUM; \
					p=DIRECTION(p); \
				} while (BOARD(p)==s); \
			}

			APPLY_8_PAIR_DERECTION_AND_ENUM(SCAN_FOR_STABLE_BY_LIGHTING)

#undef SCAN_FOR_STABLE_BY_LIGHTING

		}
	}

	for (pos_t pos = FIRST; pos < LAST; ++pos) {
		if (BOARD(pos)==s) {
			uchar bits=touching_wall[pos];
			if (IS_STABLE_STONE(bits)) ++cnt;
		}
	}
	return cnt;

#endif
}

void Board::init_from_str(const string& query) {
	assert(query.size() == 65);
	total[EMPTY] = 0;
	total[BLACK] = 0;
	total[WHITE] = 0;
	total[PASS] = 0;

	for_n(pos, MAP_SIZE)
	{
		if (! ON_BOARD(pos)) { //不在棋盘上，填满墙
			BOARD(pos)=WALL;
		}
	}

	for_n(x, 8)
	{
		for_n(y, 8)
		{
			uint i = x * 8 + y;
			int pos=POS(x, y);
			char c = query[i];
			if (c == '0') {
				BOARD(pos) = EMPTY;
				total[EMPTY] += 1;
			} else if (c == '1') {	//代表 BLACK
				BOARD(pos) = BLACK;
				total[BLACK] += 1;
			} else if (c == '2') {	//代表 WHITE
				BOARD(pos) = WHITE;
				total[WHITE] += 1;
			} else
				assert(false);
		}
	}
	color turn = (query[64] == '1') ? BLACK : WHITE;

	int pointer = (60 - 1) - total[EMPTY];	//指向历史中的最后一个有效元素
	if (pointer >= 0)
		history[pointer].turn = OPPO(turn);	//TODO: 只能恢复到上一次下子时的颜色

	update_possible_moves(turn);
	init_board_hash();
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

	init_board_hash();
}

void Board::init_board_hash() {
	hash=0;
	for_n(pos, MAP_SIZE)
	{
		//针对当前位置更新hash
		color c=BOARD(pos);
		if (c==ACTIVE) c=EMPTY;//忽略激活状态，当做空格
		hash ^= board_hash_table[pos][c];
	}

	//根据turn更新hash
	color t=turn();
	hash ^= board_hash_table[MAP_SIZE][t];
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
	int pointer = (60 - 1) - total[EMPTY];	//指向历史中的最后一个有效元素
	pos_t last_pos=PASS;
	if (pointer>=0) last_pos=history[pointer].pos;
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
			pos_t p=POS(i, j);
			const color& c = BOARD(p);
			if (c == EMPTY)
				o << '.';
			else if (c == BLACK)
				o << 'X';
			else if (c == WHITE)
				o << 'O';
			else
				o << '*';

			pos_t p_east=EAST(p);
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
	o << " empty=" << (uint) total[EMPTY] << " active=" << (uint) total[ACTIVE];
	o << " pass=" << (uint) total[PASS] << " turn=" << COLOR(turn);
	o << " hash=0x"<< std::hex << hash << std::dec;
	o << endl;

	o << "black_stable=" << get_stable_stones_size(BLACK);
	o << " white_stable=" << get_stable_stones_size(WHITE);
	o << " win=" << evaluate_and_predict_win_score();
	o << endl;

	o << "history=";
	for_n(i, pointer+1) o<<history[i]<<' ';
	o << endl;

	return o << "--------------------------------------------" << endl;
}


