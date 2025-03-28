#ifndef SEARCH_H_
#define SEARCH_H_
/*
 * File: search.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年3月12日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "board.h"

int negamax_raw() {
	// 当前最佳估值，预设为负无穷大
	int best_value = INT32_MIN;
	// 尝试每个下棋位置
	for (int pos=A1; pos<=H8; ++pos) {
		// 试着下这步棋，如果棋步合法
		if (make_move(pos)) {
			// 对所形成的局面进行评估
			int value = -evaluation();
			// 恢复原来的局面
			undo_move(pos);
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

int negamax(int depth, int pass) {
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
			int value = -negamax(depth-1, 0);
			// 恢复原来的局面
			undo_move(pos);
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
	// log_status("best="<<best_value<<", depth="<<depth);
	return best_value;
}

int alpha_beta(int alpha, int beta, int depth, int pass){
	// 当前最佳估值，预设为负无穷大
	int best_value = INT32_MIN;
	// 如果到达预定的搜索深度
	if (depth <= 0) {
		// 深度搜索完了，不用再展开了，直接给出估值
		search_leafs_eval++;
		return evaluation();
	}
	// 尝试每个下棋位置
	for (int pos=A1; pos<=H8; ++pos) {
		// 试着下这步棋，如果棋步合法
		if (make_move(pos)) {
			// 对所形成的局面进行递归搜索
			int value = -alpha_beta(-beta, -alpha, depth-1, 0);
			// 恢复原来的局面
			undo_move(pos);
			// 如果这步棋引发剪枝
			if (value >= beta) {
				// 停止对当前局面的搜索，立即返回
				search_cut_branchs++;
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
			search_leafs_exact++;// TODO: use exact_cnt
			return get_exact();
		}
		// 否则这步棋弃着
		pass_move();
		// 递归搜索，并标明该步弃着
		best_value = -alpha_beta(-beta, -alpha, depth, 1); //换对手走棋，是不是应该不是负号了
		// 恢复原来的局面
		unpass_move();
	}
	// 返回最佳估值
	search_nodes++;
	return best_value;
}

int pvs(int alpha, int beta, int depth, int pass){
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
					undo_move(pos);
					// 舍弃这步棋
					continue;
				}
				// 如果这步棋引发剪枝
				if (value >= beta) {
					// 恢复原来的局面
					undo_move(pos);
					// 停止对当前局面的搜索，立即返回
					return value;
				}
				// 保存当前结果以备下一步常规搜索使用
				best_value = alpha = value;
			}
			// 进行常规递归搜索
			int value = -pvs(-beta, -alpha, depth-1, 0);
			// 恢复原来的局面
			undo_move(pos);
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
			// if (op==0) print_board();
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
int deepening(int seconds) {
	TIMER_START(now);
	int value = 0;
	// 初始搜索深度
	int depth = 1;
	float elapsed;
	ulong total_evals=0;
	do {
		// 进行常规的MTD(f)算法
		search_nodes = 0;
		search_leafs_eval = 0;
		search_leafs_exact = 0;
		search_cut_branchs = 0;
		value = mtd(value, depth);
		elapsed=TIMER_ELASPED(now);
		total_evals+=search_leafs_eval;
		float total_speed = 1.0*total_evals/elapsed;
		// printf("depth=%d played=%d nodes=%d evals=%d exacts=%d cuts=%d eval seconds=%f value=%d \n", 
		// 	depth, (played_cnt+depth), search_nodes, search_leafs_eval, search_leafs_exact, search_cut_branchs, elapsed, value);
		printf("depth=%d final=%d nodes=%llu evals=%llu cuts=%llu tevals=%llu seconds=%0.5f speed=%0.1f value=%d \n", 
			depth, (played_cnt+depth), search_nodes, search_leafs_eval, search_cut_branchs, total_evals, elapsed, total_speed, value);
		// 增加搜索深度
		++depth;
	// 直到时间用完
	} while (elapsed < seconds && depth<=empty_cnt) ;
	return value;
}

int fast_play(uint n) { // play on first valid move
	for_n(i, n) {
		int made_move=0;
		for (int pos=A1; pos<=H8; ++pos) {
			if (make_move(pos)) {
				if (game_over()) return 0;
				made_move=1;
				break;
			}
		}
		if (made_move==0) {
			pass_move();
			if (game_over()) return 0;
		}
	}
	return 1;
}

int rand_play(uint n) {//TODO: to be check and fixed 
	if (n==0) return -1; // no need to play

	int verbose = 0;
	while (!game_over() && n>0) {
		if (verbose) {
			print_board();
			// getchar();
		}
		int valid_cnt = 0;
		for (int pos_idx=A1; pos_idx<=H8*3; ++pos_idx) {//try 3x64 times rand play
			uint pos = rand() % SIZE;
			if (valid_move(pos)) {
				valid_cnt++;
//				printf("make move from %c%c, played=%d, empty=%d turn=%s \n", TEXT(pos), played_cnt, empty_cnt, COLOR(turn));
				if (verbose) { 
					printf("%s is going to make move on %c%c\n", COLOR(turn), TEXT(pos));
				}
				make_move(pos);
				if (verbose) {
					getchar();
				}
				break;	
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
		n--;
	}
	return n==0 ? 1 : 0; // 1: finished played n moves, 0: game over before finished n moves
}



#endif /* SEARCH_H_ */
