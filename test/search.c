/**
 * File: search.c
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-02-20 21:08:58.494853
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "stdio.h"
#include "stdlib.h"

char BLACK='X', WHITE='O', EMPTY=' ';
int LEN=8;
char board[64];
int A1=0, H8=63;

int negamax() {
	// 当前最佳估值，预设为负无穷大
	int best_value = -INF_VALUE;
	// 尝试每个下棋位置
	for (int pos=A1; pos<=H8; ++pos) {
		// 试着下这步棋，如果棋步合法
		if (make_move(pos)) {
			// 对所形成的局面进行评估
			int value = -evaluation();
			// 恢复原来的局面
			unmake_move(pos);
			// 如果这步棋更好
			if (value > best_value) {
				// 保存更好的结果
				best_value = value;
			}
		}
	}
	// 如果没有合法棋步
	if (best_value == -INF_VALUE) {
		// 直接评估当前局面
		best_value = evaluation();
	}
	// 返回最佳估值
	return best_value;
}

int main (int argc, char* argv[]) {
	int a=1;
	printf("hello%d", a);
	return 0;
}

