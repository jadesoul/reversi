#ifndef BOARD_H_1371483294_09
#define BOARD_H_1371483294_09
/**
 * File: board.h
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2013-06-17 23:34:54.092000
 * Written In: Peking University, beijing, China
 */

#include "bootstrap.h"
#include "logging.h"
#include "random.h"
#include "thread.h"

#include <boost/timer.hpp>
using boost::timer;

//棋子状态
typedef uchar color;
const static color EMPTY=0;
const static color DRAW=0;//平局
const static color WHITE=1;
const static color BLACK=2;
const static color ACTIVE=3;

//8个方向
typedef uchar byte;
const static byte RIGHT=0;
const static byte RIGHT_DOWN=1;
const static byte DOWN=2;
const static byte LEFT_DOWN=3;
const static byte LEFT=4;
const static byte LEFT_UP=5;
const static byte UP=6;
const static byte RIGHT_UP=7;

//在各个方向上根据当前坐标计算下一个坐标，根据横纵坐标增量表计算
const static int INC_X[8]={1, 1, 0, -1, -1, -1, 0, 1};
const static int INC_Y[8]={0, -1, -1, -1, 0, 1, 1, 1};
#define SET_NEXT(x, y, d) (x)+=INC_X[(d)]; (y)+=INC_Y[(d)];

//用于计算一个方向的反方向
const static byte INV_D[8]={LEFT, LEFT_UP, UP, RIGHT_UP, RIGHT, RIGHT_DOWN, DOWN, LEFT_DOWN};
#define INVERSE_DIRECTION(d) ((d+4) % 8)//faster, why ?
//#define INVERSE_DIRECTION(d) INV_D[d]//slower

//获取对手的颜色
#define OPPO(x) (ACTIVE-x)

//放弃下子的move表示方法
#define PASS 0xFF

class Board {
public:
	color 	map[8][8];	//存放64个棋子状态
	uchar 	total[4];		//存放4种颜色棋子的个数并动态更新，其中ACTIVE的个数与EMPTY个数重叠
	color 	turn;			//当前轮到哪方下子
	uchar 	pass_cnt;		//PASS次数
	
	Board() {
		init_board_map();
		turn=BLACK;//黑子先下
		clear_pass_cnt();
		update_possible_moves(turn);
	}
	
	inline void clear_pass_cnt() { pass_cnt=0; }
	inline uchar mobility() const { return total[ACTIVE]; }
	inline uchar empty_cnt() const { return total[EMPTY]; }
	inline uchar black_cnt() const { return total[BLACK]; }
	inline uchar white_cnt() const { return total[WHITE]; }
	
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
		turn=(query[64]=='1')?BLACK:WHITE;
		clear_pass_cnt();
		update_possible_moves(turn);
	}
	
	//构造初始局面
	inline void init_board_map() {
		memset(this, 0, sizeof(*this));
		map[3][4]=map[4][3]=BLACK;
		map[4][4]=map[3][3]=WHITE;
		
		total[BLACK]=total[WHITE]=2;
		total[EMPTY]=60;
		total[ACTIVE]=0;
	}

	inline bool game_over() { //无子可下，或者连续两次PASS
		return empty_cnt()==0 or pass_cnt>=2;
	}

	//清除局面上所有的ACTIVE状态
	inline void clear_active_states() {
		color* base=&map[0][0];
		for_n(i, 64) if (base[i]==ACTIVE) base[i]=EMPTY;
		total[ACTIVE]=0;
	}

	//给定上一次落子方的颜色
	//重新生成（更新）局面上所有的ACTIVE位置
	//返回所有被激活的ACTIVE位置数，也就是行动力值
	uint update_possible_moves(color s) {
		clear_active_states();//先清除状态
		uint mobility=0;//行动力值
		for_n(i, 8) {
			for_n(j, 8) {
				color& c=map[i][j];
				if (c!=s) continue;//寻找所有自己的子
				color o=OPPO(s);//对方棋子颜色
				//向八个方向探索是否可以吃子
				for_n(d, 8) {
					uint x=i, y=j;
					uint cnt=0;//该方向上能吃几个子
					SET_NEXT(x, y, d);
					while (x<8 and y<8 and map[x][y]==o) {//注意x, y都是uint，一定非负
						cnt+=1;//累加吃子数
						SET_NEXT(x, y, d);//探测下一个位置
					}
					if (x<8 and y<8) {
						//没碰墙，要么是自己的子，要么是空子，或者已经是ACTIVE
						if (map[x][y]==EMPTY and cnt>0) {//如果是空子且能吃子，则激活
							map[x][y]=ACTIVE;
							mobility+=1;//新增一个行动力
						} else {
							//已激活或者是自己的子
							continue;
						}
					} else {
						//越界，碰到墙了
						continue;
					}
				}
			}
		}
		log_info("mobility="<<mobility);
		total[ACTIVE]=mobility;
		return mobility;
	}

	void dump(ostream& o=cout) {
		o<<endl<<'+'<<' ';
		for_n(j, 8) o<<j<<' ';
		o<<endl;
		for_n(i, 8) {
			o<<i<<' ';
			for_n(j, 8) {
				color& c=map[i][j];
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
		o<<" turn=";
		if (turn==BLACK) o<<"BLACK";
		else o<<"WHITE";
		o<<" PASS="<<(int)pass_cnt;
		o<<endl;
		o<<"--------------------------------------------"<<endl;
	}
	
	friend inline ostream& operator<<(ostream& o, Board& b) {
		b.dump(o);
		return o;
	}
	
	//设置指定位置的棋子颜色，本函数仅用于布局
	inline void set(uint i, uint j, color c) {
		assert(c==BLACK or c==WHITE or c==EMPTY);
		
		color orig=map[i][j];
		if (orig==c) return;
		
		total[orig]-=1;
		if (orig==ACTIVE) total[EMPTY]-=1;
		
		map[i][j]=c;
		
		total[c]+=1;
	}

	//turn方PASS，放弃下子，需满足ACTIVE个数为0
	inline void pass() {
		assert(total[ACTIVE]==0);
		log_info("pass ...");
		swap_turn();//交换下子方
		++pass_cnt;
		update_possible_moves(turn);
	}
	
	//交换下子方
	inline void swap_turn() {
		log_info("swap turn ...");
		turn=OPPO(turn);
	}
	
	inline uint play(uchar move) {
		uint x=move>>4, y=move&0x0F;
		return play(x, y);
	}

	uchar get_first_move() {
		for_n(x, 8) {
			for_n(y, 8) {
				if (map[x][y]==ACTIVE) {
					uchar move=(x<<4)+y;
					return move;
				}
			}
		}
		return PASS;
	}

	//在指定的位置放置指定颜色的棋子，检查是否合法
	//若不合法则返回0，否则返回吃子数，吃子数一定不是0
	uint play(uint i, uint j) {
		color& s=turn;
		
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
			SET_NEXT(x, y, d);
			while (x<8 and y<8 and map[x][y]==o) {//注意x, y都是uint，一定非负
				++cnt;//累加吃子数
				SET_NEXT(x, y, d);//探测下一个位置
			}
			if (x<8 and y<8) {//没碰墙
				if (map[x][y]==s and cnt>0) {//如果是自己的子且可吃子，则吃子
					uint d_inverse=INVERSE_DIRECTION(d);//向相反的方向顺序扫描并吃子
					SET_NEXT(x, y, d_inverse);
					while (map[x][y]==o) {//一定不会越界，直到遇到下子点才结束
						map[x][y]=s;
						SET_NEXT(x, y, d_inverse);
					}
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
		log_info("flip stones="<<all_cnt);
		
		return all_cnt;
	}
};


#endif /* BOARD_H_1371483294_09 */
