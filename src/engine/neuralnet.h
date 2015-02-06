#ifndef NEURALNET_H_1421152572_56
#define NEURALNET_H_1421152572_56
/**
 * File: neuralnet.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-13 20:36:12.560296
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "board.h"
#include "game.h"

#define HIDDEN_SIZE 64//隐藏层的节点个数
typedef float real;

#define EXP_TABLE_SIZE 1000			//sigmoid缓存表大小
#define MAX_EXP 6					//sigmoid函数的输入小于-6时，值接近0，大于6时，值接近1，在这个范围内的输入缓存起来，超过范围直接给出取值

class NeuralNetwork {
private:
	color X[64]; //输入层，对应棋盘上的64个棋子，各个棋子的取值可以是BLACK,WHITE,EMPTY
	real XH[64][HIDDEN_SIZE]; //输入层到隐藏层的权重矩阵XH[i][j]代表weight(X[i], H[j])
	real H[HIDDEN_SIZE]; //隐藏层节点的取值
	real H_ERR[HIDDEN_SIZE]; //隐藏层节点的误差
	real HY[HIDDEN_SIZE][64]; //输入层到隐藏层的权重矩阵HY[j][k]代表weight(H[j], Y[k])
	real Y[64]; //输出层，对应棋盘上64个棋子，在各个位置下子的得分
	real alpha; //学习率
	real expTable[EXP_TABLE_SIZE];	//存放预先计算出的sigmoid函数值

public:
	NeuralNetwork();

	void train();

	pos_t predict(const Board& board);

	ostream& dump(ostream& o = cout) const;
	friend inline ostream& operator<<(ostream& o, const NeuralNetwork& n) { return n.dump(o); }

protected:
	inline void read_input(const Board& board) { for_n(i, 64) X[i]=board.get_stone_color(POS(i/8, i%8)); }

	void reset();
	void train_one_move(const Board& board, pos_t pos);

	inline void reset_hidden() { for_n(i, HIDDEN_SIZE) H[i] = H_ERR[i] = 0; }

	void fill_sigmoid_table();

	real sigmoid(real x);
};

#endif /* NEURALNET_H_1421152572_56 */
