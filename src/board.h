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

//if we can make move, return true
int make_move(int pos) {
	assert(is_empty(pos));

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

	//play move
	op ^= flip;
	my ^= flip;
	SET_BIT(my, pos);

	//update counters
//	my_cnt += count1(flip) + 1;
//	op_cnt -= count1(flip);
	--empty_cnt;
//	pass_cnt = 0;
	++played_cnt;//increment index in history

	SWAP_TURN();

	return 1;
}

int undo_move(int pos) {
	//recover from last
	if (played_cnt==0) return 0;

	--played_cnt;//decrement index in history
//	move* last = &history[played_cnt];//get last move
	ulong flip=history[played_cnt];
//	turn = last->turn;
//	oppo = OPPO(turn);
//	int pos = last->pos;
//	win = last->win;
//	ulong eat_mask = last->flip;

	//play move
	my ^= flip;
	op ^= flip;
	CLEAR_BIT(op, pos);

	//update counters
//	total[turn] -= last.eat + 1;
//	total[oppo] += last.eat;

	++empty_cnt;

//	pass_cnt = 0;//TODO, should this be stored in Move

	SWAP_TURN();

	return 1;
}


#endif /* BOARD_H_ */
