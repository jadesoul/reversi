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

#define INPUT_SIZE (64*4)		//输入层的节点个数
#define HIDDEN_SIZE 2560		//隐藏层的节点个数

#if defined(TRAIN_MODE_WITH_WIN)
#define OUTPUT_SIZE 7	//输出层的节点个数 [-64, 0, +63], 7bits
#elif defined(TRAIN_MODE_WITH_WIN_ONE)
#define OUTPUT_SIZE (64+1+64)	//输出层的节点个数 [-64, 0, +64]
#endif

typedef float real;

#define EXP_TABLE_SIZE 1000			//sigmoid缓存表大小
#define MAX_EXP 6					//sigmoid函数的输入小于-6时，值接近0，大于6时，值接近1，在这个范围内的输入缓存起来，超过范围直接给出取值

//输入棋盘64是否黑+64是否白，128bits，经过隐藏层，输出当前局面的评分[-64, 64]
//预测出当前局面的估值：黑子的赢子数
class NeuralNetwork {
private:
	//输入层，对应棋盘上的64个棋子
	uint X[INPUT_SIZE];//X[0:64]代表每个位置上是否是黑子，
						//X[64:64*2]代表每个位置上是否是白子
						//X[64*2:64*3]代表每个位置上是否是空
						//X[64*3:64*4]代表每个位置上是否可下子

	real XH[INPUT_SIZE][HIDDEN_SIZE]; //输入层到隐藏层的权重矩阵XH[i][j]代表weight(X[i], H[j])
	real H[HIDDEN_SIZE]; //隐藏层节点的取值
	real H_ERR[HIDDEN_SIZE]; //隐藏层节点的误差
	real HY[HIDDEN_SIZE][OUTPUT_SIZE]; //输入层到隐藏层的权重矩阵HY[j][k]代表weight(H[j], Y[k])

#if defined(TRAIN_MODE_WITH_POS)
	real Y[OUTPUT_SIZE];//输出当前局面的最佳下子位置（不太靠谱）
#elif defined(TRAIN_MODE_WITH_WIN)
	real Y[OUTPUT_SIZE];//输出当前局面的评分[-64, 63], 7bits, [0 -64, 127 - 64]
#elif defined(TRAIN_MODE_WITH_WIN_ONE)
	real Y[OUTPUT_SIZE];//输出当前局面的评分[-64, 64], 129bits，只有1位取1代表取对应的估值，其他都是0
#endif

	real alpha; //学习率
	real expTable[EXP_TABLE_SIZE];	//存放预先计算出的sigmoid函数值

public:
	NeuralNetwork();
	NeuralNetwork(const string& fp);

	//自动组织比赛，根据结果动态产生训练数据进行训练
	void train();

	//从文件中读取已经生成好的比赛结果，进行训练
	void train(const string& fp);

//	void test(const string& fp);

	pos_t predict(const Board& board);

	void save_model(const string& fp) const;
	ostream& dump(ostream& o = cout) const;
	friend inline ostream& operator<<(ostream& o, const NeuralNetwork& n) { return n.dump(o); }

	void load_model(const string& fp);
	istream& from(istream& i = cin);
	friend inline istream& operator>>(istream& i, NeuralNetwork& n) { return n.from(i); }

	real get_avg_weight() const;
	real get_weight_square(real avg) const;

protected:
	void read_input(const Board& board);

	void reset();
	void train_one_move(const Board& board, const Move& move);

	inline void reset_hidden() { for_n(i, HIDDEN_SIZE) H[i] = H_ERR[i] = 0; }

	void fill_sigmoid_table();

	real sigmoid(real x);
};

#if defined(TRAIN_MODE_WITH_WIN) || defined(TRAIN_MODE_WITH_WIN_ONE)
extern NeuralNetwork g_network;
#endif
#endif /* NEURALNET_H_1421152572_56 */
