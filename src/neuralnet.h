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

class NeuralNetwork {
private:
	color X[64];//输入层，对应棋盘上的64个棋子，各个棋子的取值可以是BLACK,WHITE,EMPTY
	float XH[64][HIDDEN_SIZE];//输入层到隐藏层的权重矩阵XH[i][j]代表weight(X[i], H[j])
	float H[HIDDEN_SIZE];//隐藏层节点的取值
	float H_ERR[HIDDEN_SIZE];//隐藏层节点的误差
	float HY[HIDDEN_SIZE][64];//输入层到隐藏层的权重矩阵HY[i][j]代表weight(H[i], Y[j])
	float Y[64];//输出层，对应棋盘上64个棋子，在各个位置下子的得分

public:
	NeuralNetwork() {
		reset();
	}

	void reset() {
		for_n(i, 64) X[i]=EMPTY;
		for_n(i, 64) for_n(j, HIDDEN_SIZE) XH[i][j]=0;
	}
};

#endif /* NEURALNET_H_1421152572_56 */
