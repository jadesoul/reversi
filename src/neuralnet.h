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

#include "ai.h"

#define HIDDEN_SIZE 200//隐藏层的节点个数
typedef float real;

class NeuralNetwork {
private:
	color X[64];//输入层，对应棋盘上的64个棋子，各个棋子的取值可以是BLACK,WHITE,EMPTY
	real XH[64][HIDDEN_SIZE];//输入层到隐藏层的权重矩阵XH[i][j]代表weight(X[i], H[j])
	real H[HIDDEN_SIZE];//隐藏层节点的取值
	real H_ERR[HIDDEN_SIZE];//隐藏层节点的误差
	real HY[HIDDEN_SIZE][64];//输入层到隐藏层的权重矩阵HY[i][j]代表weight(H[i], Y[j])
	real Y[64];//输出层，对应棋盘上64个棋子，在各个位置下子的得分

public:
	NeuralNetwork() {
		srand((int) time(NULL)); //初始化随机数种子
		reset();
	}

	void train() {
		while (1) {
//			RandomAIPlayer black, white;
//			Game game(black, white);

			//先拿开局库试水
			map<Board, Choices>& book=openings->book;
			for (pair<const Board, Choices>& entry : book) {
				const Board& board=entry.first;
				Choices& choices=entry.second;

				//拷贝棋盘
				memcpy((void*)X[0], (const void*)board.map[0][0], 64);

				for (pair<const move_t, double>& choice : choices) {
					move_t move=choice.first;
					double score=choice.second;

					reset_hidden();
				}
			}
		}
	}

private:
	void reset() {
		//空子
		for_n(i, 64) {
			X[i]=EMPTY;
			Y[i]=0;
		}

		//随机初始化权重
		for_n(i, 64) for_n(j, HIDDEN_SIZE) {
			XH[i][j]=(rand() / (real)RAND_MAX - 0.5) / HIDDEN_SIZE;
			HY[j][i]=0;//(rand() / (real)RAND_MAX - 0.5) / HIDDEN_SIZE;
		}

		reset_hidden();
	}

	void reset_hidden() {
		for_n(i, HIDDEN_SIZE) {
			H[i]=0;
			H_ERR[i]=0;
		}
	}
};


class NeuralNetworkTest {
public:
	NeuralNetwork network;

	NeuralNetworkTest() {

	}
};

#endif /* NEURALNET_H_1421152572_56 */
