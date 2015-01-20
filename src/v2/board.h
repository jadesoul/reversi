#ifndef BOARD_H_1421483226_58
#define BOARD_H_1421483226_58
/**
 * File: board.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-17 16:27:06.583562
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "../bootstrap.h"

//重新实现棋盘类，实现高性能，以及局面hash

//typedef unsigned char uchar;
//typedef unsigned int uint;
typedef uint color;

//棋子的颜色状态
#define DRAW	0//平局
#define EMPTY	0//空格
#define WHITE	1//白子
#define BLACK	2//黑子
#define ACTIVE	3//激活状态，可下子
#define WALL	4//墙壁，用于判断棋子出界

//给定自己棋子的颜色，获取对手的颜色
#define OPPO(c) (BLACK+WHITE-(c))

//棋盘边长
#define BOARD_LEN		8
//总棋格数
#define BOARD_SIZE		(BOARD_LEN * BOARD_LEN)

/*棋盘上方、左方、下方各加上一面墙后形成地图
 * 	墙上的子姑且叫墙子
 *	墙用来判断棋子是否出界
 *	末尾追加一个墙子，用于判断棋盘上最后一颗子的右下方边界
 */
//地图的宽度
#define MAP_WIDTH 		(BOARD_LEN + 1)
//地图的高度
#define MAP_HEIGHT 		(BOARD_LEN + 2)
//地图的总棋格数
#define MAP_SIZE 		(MAP_WIDTH * MAP_HEIGHT + 1)
//去掉首尾行墙壁后的主体部分棋格数
#define BOARD_BODY		(MAP_WIDTH * BOARD_LEN)

//去掉第一行的墙，和第二行的第一个墙子，棋盘上第一个棋子在地图上的偏移量
#define FIRST			(MAP_WIDTH + 1)
//去掉第一行墙，和所有棋子，棋盘最后一颗棋子的偏移量
#define LAST			(MAP_WIDTH + BOARD_BODY)

//是否是棋子颜色（而非辅助性颜色）
#define IS_STONE(color)	((color)==WHITE || (color)==BLACK)

//Note that POS(-1, -1) == 0
#define POS(i, j)		(FIRST + (i) * MAP_WIDTH + (j))
#define OFFSET(i, j)	((i)*BOARD_LEN + (j))
//DELTA() is defined so that POS(i+di, j+dj) = POS(i, j) + DELTA(di, dj).
#define DELTA(di, dj)	((di) * MAP_WIDTH + (dj))
#define I(pos)			((pos) / MAP_WIDTH - 1)
#define J(pos)			((pos) % MAP_WIDTH - 1)
#define PASS			0
#define N				(-MAP_WIDTH)
#define S				MAP_WIDTH
#define W				(-1)
#define E				1
#define SOUTH(pos)		((pos) + S)
#define NORTH(pos)		((pos) + N)
#define WEST(pos)		((pos) + W)
#define EAST(pos)		((pos) + E)
#define SW(pos)			((pos) + S + W)
#define SE(pos)			((pos) + S + E)
#define NW(pos)			((pos) + N + W)
#define NE(pos)			((pos) + N + E)
#define SS(pos)			((pos) + 2 * S)
#define NN(pos)			((pos) + 2 * N)
#define WW(pos)			((pos) + 2 * W)
#define EE(pos)			((pos) + 2 * E)
#define	DELTA_8			N, N+E, E, S+E, S, S+W, W, N+W

#define ON_BOARD2(i, j)	((unsigned)(i)<BOARD_LEN and (unsigned)(j)<BOARD_LEN)
#define ON_BOARD(pos)	ON_BOARD2(I(pos), J(pos))

//直接邻居
#define DIRECT_NEIGHBORS(pos1, pos2)		\
			((pos1) == SOUTH(pos2)			\
			|| (pos1) == WEST(pos2)			\
			|| (pos1) == NORTH(pos2)		\
			|| (pos1) == EAST(pos2))

//对角邻居
#define DIAGONAL_NEIGHBORS(pos1, pos2)		\
			((pos1) == SW(pos2)				\
			|| (pos1) == NW(pos2)			\
			|| (pos1) == NE(pos2)			\
			|| (pos1) == SE(pos2))

#define BOARD(pos)				data[pos]
#define IS_COLOR(pos, color)	(BOARD(pos)==(color))
#define IS_WALL(pos)			IS_COLOR(pos, WALL)
#define IS_EMPTY(pos)			IS_COLOR(pos, EMPTY)
#define IS_BLACK(pos)			IS_COLOR(pos, BLACK)
#define IS_WHITE(pos)			IS_COLOR(pos, WHITE)

//根据与两条坐标轴夹角对角线对折后的新坐标
#define MIRROR_XY(pos)			POS(J(pos), I(pos))
//根据与两条坐标轴夹角对角线垂直的另一条对角线对折后的新坐标
#define MIRROR_IJ(pos)			POS(BOARD_LEN - 1 - J(pos), BOARD_LEN - 1 - I(pos))
//先后根据两条对角线对折后的新坐标
#define MIRROR_IY(pos)			POS(BOARD_LEN - 1 - I(pos), BOARD_LEN - 1 - J(pos))

#define END (uchar)(-1)
#define COLOR(turn) ((turn)==BLACK?"BLACK":"WHITE")

#define M		(BOARD_LEN-1)
#define LU		POS(0,0)
#define RU		POS(0,M)
#define LD		POS(M,0)
#define RD		POS(M,M)

#define IS_LU_CORNER(pos)			((pos)==LU)
#define IS_RU_CORNER(pos)			((pos)==RU)
#define IS_LD_CORNER(pos)			((pos)==LD)
#define IS_RD_CORNER(pos)			((pos)==RD)
#define IS_GOOD_CORNER(pos)			(IS_LU_CORNER(pos) \
									or IS_RU_CORNER(pos) \
									or IS_LD_CORNER(pos) \
									or IS_RD_CORNER(pos))

#define LU1		SE(LU)
#define LU2		SOUTH(LU)
#define LU3		EAST(LU)
#define RU1		SW(RU)
#define RU2		SOUTH(RU)
#define RU3		WEST(RU)
#define LD1		NE(LD)
#define LD2		NORTH(LD)
#define LD3		EAST(LD)
#define RD1		NW(RD)
#define RD2		NORTH(RD)
#define RD3		WEST(RD)

#define BESIDE_LU_CORNER(pos)		((pos)==LU1 or (pos)==LU2 or (pos)==LU3)
#define BESIDE_RU_CORNER(pos)		((pos)==RU1 or (pos)==RU2 or (pos)==RU3)
#define BESIDE_LD_CORNER(pos)		((pos)==LD1 or (pos)==LD2 or (pos)==LD3)
#define BESIDE_RD_CORNER(pos)		((pos)==RD1 or (pos)==RD2 or (pos)==RD3)

#define BESIDE_GOOD_CORNER(pos)		(BESIDE_LU_CORNER(pos) \
									or BESIDE_RU_CORNER(pos) \
									or BESIDE_LD_CORNER(pos) \
									or BESIDE_RD_CORNER(pos))

#define APPLY_8_DIRECTIONS(MACRO)	\
									MACRO(EAST) \
									MACRO(WEST) \
									MACRO(NORTH) \
									MACRO(SOUTH) \
									MACRO(NW) \
									MACRO(NE) \
									MACRO(SW) \
									MACRO(SE)

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(a, b) ((a)<(b) ? (a) : (b))
#define MAX(a, b) ((a)<(b) ? (b) : (a))

//一步棋，包括落子位置，轮谁下等
struct Move {
	color turn;
	int pos;

	Move(color turn=BLACK, int pos=PASS):turn(turn), pos(PASS) {}
	Move(const char* two_bytes[2]);

	ostream& dump(ostream& o=cout) const;
	friend inline ostream& operator<<(ostream& o, const Move& m) { return m.dump(o); }
};

class Board {
public:
	Board() {
		init_board_map();
//		update_possible_moves(turn);
	}
	~Board();

	inline uchar mobility() const { return total[ACTIVE]; }
	inline uchar empty_cnt() const { return total[EMPTY]; }
	inline uchar black_cnt() const { return total[BLACK]; }
	inline uchar white_cnt() const { return total[WHITE]; }
	inline bool is_active(uchar x, uchar y) const { return is_active(POS(x, y)); }
	inline bool is_active(int pos) const { return BOARD(pos)==ACTIVE; }

	bool operator <(const Board& another) const {
		const Board* a=this;
		const Board* b=&another;
		if (a==b) return true;
		int diff=memcmp(a, b, sizeof(Board));
		return diff<0;
	}

	//将64个棋子的状态按照xy夹角对应的对角线镜像变换
	void mirror_xy() {
		for_n(x, 8) {
			for_n(y, 8) {
				if (x>=y) continue;
				int pos=POS(x, y);
				swap(data[pos], data[MIRROR_XY(pos)]);
			}
		}
	}

	//将64个棋子的状态按照左下-右上对角线镜像变换
	void mirror_ldru() {
		for_n(x, 8) {
			for_n(y, 8) {
				if (x+y>=7) continue;
				int pos=POS(x, y);
				swap(data[pos], data[MIRROR_IJ(pos)]);
			}
		}
	}

	//将64个棋子的状态按照左下-右上对角线镜像变换，同时按照xy夹角对应的对角线镜像变换
	void mirror_ldru_xy() {
		mirror_ldru();
		mirror_xy();
	}

	ostream& dump(ostream& o=cout) const {
		color turn = get_current_turn();
		char h = (turn==BLACK?'A':'a');
		o<<endl<<'+'<<' ';
		for_n(j, 8) o<<(char)(h+j)<<' ';
		o<<endl;
		for_n(i, 8) {
			o<<(i+1)<<' ';
			for_n(j, 8) {
				const color& c=BOARD(POS(i, j));
				if (c==EMPTY) o<<'.';
				else if (c==BLACK) o<<'X';
				else if (c==WHITE) o<<'O';
				else o<<'*';
				o<<' ';
			}
			o<<endl;
		}
		o<<"BLACK="<<(uint)total[BLACK]<<" WHITE="<<(uint)total[WHITE];
		o<<" EMPTY="<<(uint)total[EMPTY]<<" ACTIVE="<<(uint)total[ACTIVE];
		o<<" turn="<< COLOR(turn);
		o<<endl;
		return o<<"--------------------------------------------"<<endl;
	}

	friend inline ostream& operator<<(ostream& o, const Board& b) { return b.dump(o); }

private:
	void init_board_map() {
		for_n(pos, MAP_SIZE) {
			if (ON_BOARD(pos)) {//在棋盘上
				if (pos==POS(3,4) or pos==POS(4,3)) {
					BOARD(pos)=BLACK;
				} else if (pos==POS(3,3) or pos==POS(4,4)) {
					BOARD(pos)=WHITE;
				} else if (pos==POS(3,2) or pos==POS(2,3) or pos==POS(4,5) or pos==POS(5,4)) {
					//黑子先下的几个位置为激活状态
					BOARD(pos)=ACTIVE;
				} else {
					BOARD(pos)=EMPTY;
				}
			} else {//不在棋盘上，填满墙
				BOARD(pos)=WALL;
			}
		}
		total[EMPTY]=60;//全部空格数，包括ACTIVE状态的激活空格数
		total[WHITE]=2;//白子总数
		total[BLACK]=2;//黑子总数
		total[ACTIVE]=4;//激活空格数

		//	memset(blink, END, BOARD_SIZE);
		//	memset(wlink, END, BOARD_SIZE);

		//	wlink[0]=OFFSET(3, 4);
		//	white[OFFSET(3, 4)]=POS(3,4);
		//	wlink[OFFSET(3, 4)]=OFFSET(4, 3);
		//	white[OFFSET(4, 3)]=POS(4,3);
		//	wlink[OFFSET(4, 3)]=END;
	}


	//从包含65字节的字符串初始化, 棋盘(64字节)，下子方(1字节)
	//为游戏引擎提供此接口
	void init_from_str(const string& query) {
		assert(query.size()==65);
		total[EMPTY]=0;
		total[BLACK]=0;
		total[WHITE]=0;
		for_n(x, 8) {
			for_n(y, 8) {
				uint i=x*8+y;
				char c=query[i];
				if (c=='0') {
					map[x][y]=EMPTY;
					total[EMPTY]+=1;
				} else if (c=='1') {//1代表 BLACK
					map[x][y]=BLACK;
					total[BLACK]+=1;
				} else if (c=='2') {//2代表 WHITE
					map[x][y]=WHITE;
					total[WHITE]+=1;
				} else assert(false);
			}
		}
		color turn=(query[64]=='1')?BLACK:WHITE;

		int pointer = (60 - 1) - total[EMPTY];//指向历史中的最后一个有效元素
		if (pointer>=0) history[pointer].turn=OPPO(turn);//只能恢复到上一次下子时的颜色

		update_possible_moves(turn);
	}


	//清除局面上所有的ACTIVE状态
	void clear_active_states() {//TODO: 将所有激活状态的棋子记录下来
		for (int i=FIRST; i<LAST; ++i) if (data[i]==ACTIVE) data[i]=EMPTY;
		total[ACTIVE]=0;
	}

	//根据历史数组判断当前轮谁下
	color get_current_turn() {
		int pointer = (60 - 1) - total[EMPTY];//指向历史中的最后一个有效元素
		if (pointer==-1) return BLACK;//开局黑先
		else return OPPO(history[pointer].turn);//上一手的对手
	}

	//给定本次已经下子的落子方颜色
	//重新生成（更新）局面上所有的ACTIVE位置
	//total[ACTIVE]返回所有被激活的ACTIVE位置数，也就是行动力值
	void update_possible_moves(color s) {
		clear_active_states();//先清除状态
		uint active=0;//激活状态数=行动力值

		for (int pos=FIRST; pos<LAST; ++pos) {
			color c=BOARD(pos);
			if (c!=s) continue;//寻找所有自己的子

			color o=OPPO(s);//对方棋子颜色
			int p;//当前位置

#define SCAN(DIRECTION) \
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
			APPLY_8_DIRECTIONS(SCAN)

#undef SCAN
		}
		total[ACTIVE]=active;
	}

	//设置指定位置的棋子颜色，本函数仅用于布局
	inline void set(uchar pos, color c) {
		assert(c==BLACK or c==WHITE or c==EMPTY);

		color orig=BOARD(pos);
		if (orig==c) return;

		total[orig]-=1;
		if (orig==ACTIVE) total[EMPTY]-=1;

		BOARD(pos)=c;

		total[c]+=1;
		//TODO: history已经乱了
	}

	//turn方PASS，放弃下子，需满足ACTIVE个数为0
	inline void pass() {
		assert(total[ACTIVE]==0);
		color oppo=OPPO(get_current_turn());//交换下子方
		update_possible_moves(oppo);
	}

	//获取第一个可以下子的位置
	int get_first_move() {
		for (int pos=FIRST; pos<LAST; ++pos) {
			if (BOARD(pos)==ACTIVE) return pos;
		}
		return PASS;
	}

	//在指定的位置放置指定颜色的棋子，检查是否合法
	//若不合法则返回0，否则返回吃子数，吃子数一定不是0
	uint play(int pos) {
		uint i=I(pos), j=J(pos);
		color s=get_current_turn();

		//落子点必须是ACTIVE状态
		color& c=map[i][j];
		if (c!=ACTIVE) return 0;

		//如果落子成功，则更新新的对手落子点
		map[i][j]=s;//首先在下子处落子
		total[ACTIVE]-=1;
		total[EMPTY]-=1;
		total[s]+=1;

		//同时将各个方向上所吃的子翻转为自己的子
		color o=OPPO(s);

		uint all_cnt=0;//记录所有方向上总吃子数
		for_n(d, 8) {
			uint x=i, y=j;
			uint cnt=0;//该方向上能吃几个子
			int inc_x=INC_X[d], inc_y=INC_Y[d];
			color* chain[8];//当前方向上吃的子存放在链中，最多能吃6个子，避免后面的反向回溯吃子

//			SET_NEXT(x, y, d);
			x+=inc_x; y+=inc_y;

			while (x<8 and y<8 and map[x][y]==o) {//注意x, y都是uint，一定非负
				chain[cnt]=&(map[x][y]);
				++cnt;//累加吃子数
//				SET_NEXT(x, y, d);//探测下一个位置
				x+=inc_x; y+=inc_y;
			}
			if (x<8 and y<8) {//没碰墙
				if (map[x][y]==s and cnt>0) {//如果是自己的子且可吃子，则吃子

//					uint d_inverse=INVERSE_DIRECTION(d);//向相反的方向顺序扫描并吃子
//					SET_NEXT(x, y, d_inverse);
//					while (map[x][y]==o) {//一定不会越界，直到遇到下子点才结束
//						map[x][y]=s;
//						SET_NEXT(x, y, d_inverse);
//					}

					//性能优化
					for_n(k, cnt) *(chain[k])=s;

					all_cnt+=cnt;//累加总吃子数
				} else {
					//不能吃子，或者是一个激活状态
					continue;
				}
			} else {
				//越界，碰到墙了
				continue;
			}
		}
		assert(all_cnt>0);
		//把对手的子翻转成了自己的子
		total[o]-=all_cnt;
		total[s]+=all_cnt;

		swap_turn();//交换下子方
		clear_pass_cnt();
		update_possible_moves(turn);
		log_debug("flip stones="<<all_cnt);

		return all_cnt;
	}

private:
	//存放64个棋子状态，以及周边的3面墙
	color data[MAP_SIZE];

	//存放4种颜色棋子的个数并动态更新，其中ACTIVE的个数与EMPTY个数重叠
	uint total[4];

	//下子历史，包括位置和颜色，约定PASS MOVE不放到历史中
	//如果有连续两个Move的color相同，说明对手PASS了
	//每一个Move对占据棋盘上的一个位置
	//历史中的有效元素数：黑子数+白子数-4 = 60 - 空格数
	Move history[BOARD_SIZE-4];

//	记录棋盘上所有的白子位置和黑子位置，加速遍历
//	uchar white[BOARD_SIZE];//存放所有已下白子的pos, 最多64个
//	uchar wlink[BOARD_SIZE];//指示下一个白子在white数组中的下标，END代表结束
//	uchar black[BOARD_SIZE];//存放所有已下黑子的pos, 最多64个
//	uchar blink[BOARD_SIZE];//指示下一个白子在black数组中的下标
};


//Game用来控制游戏结束逻辑：如果有连续两次PASS，则游戏结束

//#define SEARCH(DIRECTION) \
//			p=DIRECTION(pos); \
//			if (BOARD(p)==o) { \
//				cnt=0; \
//				while (BOARD(p)==o) { \
//					p=DIRECTION(p); \
//					++cnt; \
//				} \
//				if (BOARD(p)==EMPTY and cnt>0) { \
//					BOARD(p)=ACTIVE; \
//					++active; \
//				} \
//			}

#endif /* BOARD_H_1421483226_58 */
