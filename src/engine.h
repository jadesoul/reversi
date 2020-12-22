#ifndef ENGINE_H_
#define ENGINE_H_
/*
 * File: engine.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年3月14日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "search.h"
char response[3];

//判断输入是否合理
int valid_input(const char* query) {
	if (strlen(query) == 65) {
		if (query[64]=='1' || query[64]=='2') { // turn should in [1, 2]
			for_n(i, 64) {
				if (query[i]!='0' && query[i]!='1' && query[i]!='2') // state should in [0, 1, 2]
					return 0;
			}
			return 1;
		}
	}
	return 0;
}

//从包含65字节的字符串初始化, 棋盘(64字节)，下子方(1字节)
//为游戏引擎提供此接口
void init_from_str(const char* query) {
	assert(strlen(query) == 65);
	my=0;
	op=0;
	em=0;
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
				SET_BIT(em, pos);
				empty_cnt += 1;
			} else if (c == '1') {	// BLACK
				SET_BIT(my, pos);
				my_cnt += 1;
			} else if (c == '2') {	// WHITE
				SET_BIT(op, pos);
				op_cnt += 1;
			} else
				assert(0);
		}
	}
	played_cnt = 60-empty_cnt;
	turn = (query[64] == '1') ? BLACK : WHITE;
	oppo = OPPO(turn);

	//assume failed
	if (turn==WHITE) {
		SWAP64(my, op);
		SWAP(my_cnt, op_cnt);
	}
}

//用于游戏引擎，给定字符串（64字符的游戏局面和1个字符的turn）
//返回下子的位置坐标 (2个字符)，下标均是从1开始计算
const char* server(const char* query) {
	if (!valid_input(query)) {
		return "??";
	}

	// print_board();
	init_from_str(query);
	print_board();
	

	TIMER_START(now);
//		int mob = get_mobility();
//		if (mob==0) return "00";//表示PASS
//
//		int avg_think_time = 3.0 / mob;

	int depth = empty_cnt<=15 ? empty_cnt : 10;
	// 当前最佳估值，预设为负无穷大
	int best_value = INT32_MIN;
	int best_pos = PASS;
	// 尝试每个下棋位置

	for (int pos=A1; pos<=H8; ++pos) {
		// 试着下这步棋，如果棋步合法
		if (make_move(pos)) {
//			if (oppo==WHITE) {//choose first (random) pos for color before move
//				best_pos= pos;
//				best_value=-65;
//				break;
//			}
			// 对所形成的局面进行评估

//			int value = - deepening(avg_think_time);
			int value = - mtd(0, depth-1);
//			int value = - pvs(-64, 64, depth-1, 0);

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

	// make_move(best_pos);
	// undo_move(best_pos);
	sequence[played_cnt-1]=best_pos;
	print_board();
	
	int x=I(best_pos), y=J(best_pos);
	
	response[0]='1'+x;//从1开始编号
	response[1]='1'+y;
	response[2]=0;

	printf("%s played=%d empty=%d depth=%d pos=%c%c time=%f win=%d \n", COLOR(turn), played_cnt, empty_cnt, depth, TEXT(best_pos), TIMER_ELASPED(now), (turn==BLACK?best_value:-best_value));
	return response;
}

#endif /* ENGINE_H_ */
