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

//重新实现棋盘类，实现高性能，以及局面hash

#include "move.h"
#include "hash.h"

class Board {
public:
	Board();
	Board(const string& board);
	~Board();

	inline size_t mobility() const { return total[ACTIVE]; }
	inline size_t empty_cnt() const { return total[EMPTY]; }
	inline size_t black_cnt() const { return total[BLACK]; }
	inline size_t white_cnt() const { return total[WHITE]; }
	inline size_t pass_cnt() const { return total[PASS]; }
	inline bool is_active(uint x, uint y) const { return is_active(POS(x, y)); }
	inline bool is_active(pos_t pos) const { return BOARD(pos)==ACTIVE; }

	//获取历史上已经下的某一步
	inline Move get_history_move(int i) const { return history[i]; }

	//除去开局时棋盘的4颗固定子，到目前为止玩家一共下了几颗子
	inline size_t played_cnt() const { return 60-total[EMPTY]; }

	//到目前为止，棋盘一共有多少黑子和白子
	inline size_t stone_cnt() const { return 64-total[EMPTY]; }

	//获取棋盘局面哈希
	inline hash_t get_hash() const { return hash; }

	//获取指定位置棋子的颜色
	inline color get_stone_color(pos_t pos) const { return BOARD(pos); }

	//无子可下，或者连续两次PASS
	inline bool game_over() { return empty_cnt()==0 or pass_cnt()>=2; }

	//用于比较排序, map存储等
	bool operator <(const Board& another) const;

	//输出棋盘的字符界面
	friend inline ostream& operator<<(ostream& o, const Board& b) { return b.dump(o); }

	//将64个棋子的状态按照xy夹角对应的对角线镜像变换
	void mirror_xy();

	//将64个棋子的状态按照左下-右上对角线镜像变换
	void mirror_ldru();

	//将64个棋子的状态按照左下-右上对角线镜像变换，同时按照xy夹角对应的对角线镜像变换
	void mirror_ldru_xy();

	//设置指定位置的棋子颜色，本函数仅用于布局
	void set(pos_t pos, color c);
	inline void set(uint x, uint y, color c) { set(POS(x, y), c); }

	//turn方PASS，放弃下子，需满足ACTIVE个数为0
	void pass();

	//获取第一个可以下子的位置
	pos_t get_first_move() const;
	inline pos_t first() const { return this->get_first_move(); }

	//在指定的位置放置指定颜色的棋子，检查是否合法
	//若不合法则返回0，否则返回吃子数，吃子数一定不是0
	size_t play(pos_t pos);
	inline size_t play(uint x, uint y) { return play(POS(x, y)); }
	inline size_t play(const Move& move) { assert(move.turn==turn()); return play(move.pos); }

	//撤销上一个Move
	void undo();

	//返回所有与c的棋子相邻的空格个数,也就是潜在行动力
	size_t potential_mobility(color c) const;

	//根据历史数组判断当前轮谁下
	color get_current_turn() const;
	inline color turn() const { return this->get_current_turn(); }

	//从包含65字节的字符串初始化, 棋盘(64字节)，下子方(1字节)
	//为游戏引擎提供此接口
	void init_from_str(const string& query);

	//局面评估函数，预测当前下子方能赢多少颗子
	double evaluate_and_predict_win_score() const;

	//计算局面上有多少指定颜色的稳定子
	size_t get_stable_stones_size(color s) const;

	//计算局面上指定颜色的稳定子与对手稳定子个数之差
//	int get_stable_stones_size_diff(color s) const;

protected:
	//开局时，初始化棋盘
	void init_board_map();

	//清除局面上所有的ACTIVE状态
	void clear_active_states();

	//给定本次已经下子的落子方颜色
	//重新生成（更新）局面上所有的ACTIVE位置
	//更新完毕后调用mobility()返回所有被激活的ACTIVE位置数，也就是行动力值
	void update_possible_moves(color s);

	//初始化棋盘局面哈希
	void init_board_hash();

	//输出棋盘内容
	ostream& dump(ostream& o=cout) const;

private:
	//存放64个棋子状态，以及周边的3面墙
	color data[MAP_SIZE];

	//存放4种颜色棋子的个数并动态更新，其中ACTIVE的个数与EMPTY个数重叠
	//total[PASS=4]存放当前棋局已有几次连续PASS MOVE
	size_t total[5];

	//下子历史，包括位置和颜色，约定PASS MOVE不放到历史中
	//如果有连续两个Move的color相同，说明对手PASS了
	//每一个Move对占据棋盘上的一个位置
	//历史中的有效元素数：黑子数+白子数-4 = 60 - 空格数
	Move history[BOARD_SIZE-4];

	//存储当前局面的hash值，当棋盘发生变化时，此值会自动同步计算
	//包括局面上所有子的颜色，不包括ACTIVE，ACTIVE都当做EMPTY
	//另外还包括当前的turn，即设置一个虚拟的棋子，其颜色与当前下子方一致
	hash_t hash;

//	记录棋盘上所有的白子位置和黑子位置，加速遍历
//	uchar white[BOARD_SIZE];//存放所有已下白子的pos, 最多64个
//	uchar wlink[BOARD_SIZE];//指示下一个白子在white数组中的下标，END代表结束
//	uchar black[BOARD_SIZE];//存放所有已下黑子的pos, 最多64个
//	uchar blink[BOARD_SIZE];//指示下一个白子在black数组中的下标
};

#endif /* BOARD_H_1421483226_58 */
