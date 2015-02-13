/**
 * File: neuralnet.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-02-03 22:49:33.915929
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "neuralnet.h"
#include "ai.h"

NeuralNetwork::NeuralNetwork() {
	alpha = 0.025;
	srand((int) time(NULL)); //初始化随机数种子
	reset();
	fill_sigmoid_table();
}

void NeuralNetwork::train_one_move(const Board& board, pos_t pos) {
	read_input(board);

	uint kk = I(pos) * 8 + J(pos);
	log_debug("train nwetwork from move:"<<Move(pos, board.turn)<<", score="<<score);

	//将当前棋步作为训练数据
	reset_hidden();

	// X -> H : 每个隐藏层节点的值等于输入层向量 点乘 对应的权重向量
	log_debug("X -> H");
	for_n(j, HIDDEN_SIZE)
	{
		H[j] = 0;
		for_n(i, INPUT_SIZE)
		{
			H[j] += X[i] * XH[i][j];
		}
	}

	// H -> Y : 每个输出层节点的值等于隐藏节点向量 点乘 对应的权重向量
	log_debug("H -> Y");
	for_n(k, OUTPUT_SIZE)
	{
		real label;

		if (k == kk) {	//正样本
			label = 1;
		} else {	//负样本
			if ((rand() / (real) RAND_MAX) < 0.01)
				label = 0;
			else
				continue;
		}

		Y[k] = 0;
		for_n(j, HIDDEN_SIZE)
		{
			Y[k] += H[j] * HY[j][k];
		}

		//计算梯度
		real f = sigmoid(Y[k]);
		double g = alpha * (label - f);
		log_debug("caculate label="<<label<<", f="<<f<<", g="<<g<<", alpha="<<alpha<<", move="<<Pos(move));

		//Y -> H: 计算误差向量,后向传播误差
		log_debug("Y -> H");
		for_n(j, HIDDEN_SIZE)
		{
			H_ERR[j] += g * HY[j][k];
		}

		//更新隐藏层到输出层权重
		for_n(j, HIDDEN_SIZE)
		{
			HY[j][k] += g * H[j];
		}

		//H -> X: 误差向量累加到输入到隐藏层权重向量
		log_debug("H -> X");
		for_n(i, INPUT_SIZE)
		{
			for_n(j, HIDDEN_SIZE)
			{
				XH[i][j] += H_ERR[j];
			}
		}
	}
}

void NeuralNetwork::train() {
	log_warn("NeuralNetwork start training ...");

	uint train_with_random_games_cnt=10000;
	RandomAIPlayer black;
//	RandomAIPlayer white;
    LookNAIPlayer white;
	uint total=train_with_random_games_cnt;
	uint trained=0;
	timer previous;
	for_n(cnt, total) {
		if (cnt % 10 == 0) {
			double gap=previous.elapsed();
			log_status("games: "<<cnt<<"/"<<total<<"="<<(float(cnt)/total)
					<<" trained="<<trained
					<<" time="<<gap
					<<" speed="<<(0.001*trained/gap)<<"KB/s"
					);
		}

		Game game(black, white);
		Score score = game.start();
		int diff = score.diff();
		Board& finished=game.get_board();

		Board board;
		int pointer=0;
		while (!board.game_over()) {
			uchar mobility=board.mobility();

			if (mobility==0) {
				board.pass();
			} else {
				Move move=finished.get_history_move(pointer);
				color turn=board.get_current_turn();
//				if (move.turn!=turn) {
//					log_warn("why:"<<board);
//				}
				assert(move.turn==turn);
				pos_t pos=move.pos;

				if (turn==score.winner) {
					++trained;
					train_one_move(board, pos);
				}

				board.play(pos);
				++pointer;
			}
		}
	}

	log_warn("NeuralNetwork finished training");
}

pos_t NeuralNetwork::predict(const Board& board) {
	read_input(board);

	//将当前棋步作为训练数据
	reset_hidden();

	// X -> H : 每个隐藏层节点的值等于输入层向量 点乘 对应的权重向量
	log_debug("X -> H");
	for_n(j, HIDDEN_SIZE)
	{
		H[j] = 0;
		for_n(i, 64)
		{
			H[j] += X[i] * XH[i][j];
		}
	}

	// H -> Y : 每个输出层节点的值等于隐藏节点向量 点乘 对应的权重向量
	log_debug("H -> Y");

	real max_score = INT32_MIN;
	uint best;

	for_n(k, 64)
	{
		uint x = k / 8, y = k % 8;
		if (!board.is_active(x, y))
			continue;

		//			move_t move=Pos(k/8, k%8).tomove();
		Y[k] = 0;
		for_n(j, HIDDEN_SIZE)
		{
			Y[k] += H[j] * HY[j][k];
		}
		if (Y[k] > max_score) {
			max_score = Y[k];
			best = k;
		}
	}
	return POS(best / 8, best % 8);
}

void NeuralNetwork::save_model(const string& fp) const {
	ofstream fout(fp);
	fout<<*this;
	fout.close();
}

ostream& NeuralNetwork::dump(ostream& o) const {
	o << "===================NeuralNetwork===================" << endl;
	o << "XH[j][i]:" << endl;
	for_n(i, 64)
	{
		for_n(j, HIDDEN_SIZE)
		{
			o << XH[i][j] << ' ';
		}
		o << endl;
	}
	o << "HY[j][k]:" << endl;
	for_n(j, HIDDEN_SIZE)
	{
		for_n(k, 64)
		{
			o << HY[j][k] << ' ';
		}
		o << endl;
	}
	return o << endl;
}

void NeuralNetwork::load_model(const string& fp) {
	ifstream fin(fp);
	fin>>*this;
	fin.close();
}

istream& NeuralNetwork::from(istream& in) {
	string s;
	getline(in, s);
	getline(in, s);
	for_n(i, 64)
	{
		for_n(j, HIDDEN_SIZE)
		{
			in >> XH[i][j];
		}
		getline(in, s);
	}
	getline(in, s);
	for_n(j, HIDDEN_SIZE)
	{
		for_n(k, 64)
		{
			in >> HY[j][k];
		}
		getline(in, s);
	}
	return in;
}

void NeuralNetwork::reset() {
	log_debug("reset network");
	//空子
	for_n(i, 64)
	{
//		X[i]=EMPTY;
		Y[i] = 0;
	}

	//随机初始化权重
	for_n(i, 64)
	{
		for_n(j, HIDDEN_SIZE)
		{
			XH[i][j] = (rand() / (real) RAND_MAX - 0.5) / HIDDEN_SIZE;
			//可以初始化为0，这样前期的训练快
			//HY[j][i]=0;//(rand() / (real)RAND_MAX - 0.5) / HIDDEN_SIZE;
			//也可以随机初始化
			HY[j][i] = (rand() / (real) RAND_MAX - 0.5) / HIDDEN_SIZE;
		}
	}

	reset_hidden();
}

void NeuralNetwork::fill_sigmoid_table() {
	log_info("fill sigmoid table");
	//sigmoid(x)= 1/(1+exp(-x)) = exp(x)/(1+exp(x))
	for (int i = 0; i < EXP_TABLE_SIZE; i++) {
		expTable[i] = exp((i / (real) EXP_TABLE_SIZE * 2 - 1) * MAX_EXP); // Precompute the exp() table
		expTable[i] = expTable[i] / (expTable[i] + 1); // Precompute f(x) = x / (x + 1)
	}
}

real NeuralNetwork::sigmoid(real x) {
	if (x < -MAX_EXP)
		return 0;
	if (x > MAX_EXP)
		return 1;
	return expTable[(uint) (((x) + MAX_EXP) * (EXP_TABLE_SIZE / MAX_EXP / 2))];
}

void NeuralNetwork::read_input(const Board& board) {
	for_n(i, 64) {
		color c=board.get_stone_color(POS(i/8, i%8));
		uint& b=X[i];
		uint& w=X[i+64];
		if (c==BLACK) {
			b=1;
			w=0;
		} else if (c==WHITE) {
			b=0;
			w=1;
		} else {
			b=0;
			w=0;
		}
	}
}

