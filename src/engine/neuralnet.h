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

#define INPUT_SIZE 128	//输入层的节点个数
#define HIDDEN_SIZE 256	//隐藏层的节点个数
#define OUTPUT_SIZE 7	//输入层的节点个数
typedef float real;

#define EXP_TABLE_SIZE 1000			//sigmoid缓存表大小
#define MAX_EXP 6					//sigmoid函数的输入小于-6时，值接近0，大于6时，值接近1，在这个范围内的输入缓存起来，超过范围直接给出取值

//输入棋盘64是否黑+64是否白，128bits，经过隐藏层，输出当前局面的评分[-64, 63], 7 bits
class NeuralNetwork {
private:
//	color X[64]; //输入层，对应棋盘上的64个棋子，各个棋子的取值可以是BLACK,WHITE,EMPTY
	uint X[INPUT_SIZE];//X[0:64]代表每个位置上是否有黑子，X[64:128]代表每个位置上是否有子白

	real XH[INPUT_SIZE][HIDDEN_SIZE]; //输入层到隐藏层的权重矩阵XH[i][j]代表weight(X[i], H[j])
	real H[HIDDEN_SIZE]; //隐藏层节点的取值
	real H_ERR[HIDDEN_SIZE]; //隐藏层节点的误差
	real HY[HIDDEN_SIZE][OUTPUT_SIZE]; //输入层到隐藏层的权重矩阵HY[j][k]代表weight(H[j], Y[k])

//	real Y[64]; //输出层，对应棋盘上64个棋子，在各个位置下子的得分
	uint Y[OUTPUT_SIZE];//输出当前局面的评分[-64, 63], 7bits对应的整数代表赢多少颗子，但+64无法表示，与+63等价

	real alpha; //学习率
	real expTable[EXP_TABLE_SIZE];	//存放预先计算出的sigmoid函数值

public:
	NeuralNetwork();

	//自动组织比赛，根据结果动态产生训练数据进行训练
	void train();

	//从文件中读取已经生成好的比赛结果，进行训练
	void train(const string& fp);

	pos_t predict(const Board& board);

	void save_model(const string& fp) const;
	ostream& dump(ostream& o = cout) const;
	friend inline ostream& operator<<(ostream& o, const NeuralNetwork& n) { return n.dump(o); }

	void load_model(const string& fp);
	istream& from(istream& i = cin);
	friend inline istream& operator>>(istream& i, NeuralNetwork& n) { return n.from(i); }

protected:
	void read_input(const Board& board);

	void reset();
	void train_one_move(const Board& board, pos_t pos);

	inline void reset_hidden() { for_n(i, HIDDEN_SIZE) H[i] = H_ERR[i] = 0; }

	void fill_sigmoid_table();

	real sigmoid(real x);
};

#endif /* NEURALNET_H_1421152572_56 */
