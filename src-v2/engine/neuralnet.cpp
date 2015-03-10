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

#if defined(TRAIN_MODE_WITH_WIN) || defined(TRAIN_MODE_WITH_WIN_ONE)
NeuralNetwork g_network("/Users/jadesoul/git/reversi/model/trained3.model");
#endif

NeuralNetwork::NeuralNetwork() {
	alpha = 0.025;
	srand((int) time(NULL)); //初始化随机数种子
	reset();
	fill_sigmoid_table();

//	for_n(i, 121) {
//		real x=double(int(i)-60)/10;
//		real y=sigmoid(x);
//		log_status("sigmoid("<<x<<")="<<y);
//	}
//	getchar();
}

NeuralNetwork::NeuralNetwork(const string& fp):NeuralNetwork() {
	load_model(fp);
}

void NeuralNetwork::train_one_move(const Board& board, const Move& move) {
	pos_t pos=move.pos;
	color turn=board.get_current_turn();
	assert(move.turn==turn);
	int win=move.win;//[-64, +64]
	if (turn==WHITE) win=-win;//转换为黑子的赢子数
	read_input(board);

#ifdef TRAIN_MODE_WITH_POS
	uint kk = I(pos) * 8 + J(pos);
	log_debug("train nwetwork from move:"<<Move(pos, board.turn)<<", score="<<score);
#elif defined TRAIN_MODE_WITH_WIN
	if (win<-64) win=-64;
	if (win==64) win=63;
	uchar bits=((uchar)(win+64)) & 0x7F;
	log_debug("train nwetwork from move:"<<move<<", win="<<win<<", bits=0x"<<std::hex<<(uint)bits<<std::dec);
#elif defined TRAIN_MODE_WITH_WIN_ONE
	uint kk=win+64;
//	log_status("win="<<win<<", kk="<<kk);
#endif

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

#ifdef TRAIN_MODE_WITH_POS
		if (k == kk) {	//正样本
			label = 1;
		} else {	//负样本
			if ((rand() / (real) RAND_MAX) < 0.01)
				label = 0;
			else
				continue;
		}
#elif defined TRAIN_MODE_WITH_WIN
		label= ((bits>>k) & 0x01) ? 1 : 0;
//		log_status("k="<<k<<", label="<<label);
//		getchar();
#elif defined TRAIN_MODE_WITH_WIN_ONE
		if (k == kk) {	//正样本
			label = 1;
		}
		else {	//负样本
//			continue;
			if ((rand() / (real) RAND_MAX) < 0.01)
				label = 0;
			else
				continue;
		}
#endif

		Y[k] = 0;
		for_n(j, HIDDEN_SIZE)
		{
			Y[k] += H[j] * HY[j][k];
		}

		//计算梯度
		real f = sigmoid(Y[k]);
		real g = alpha * (label - f);
//		log_status("caculate label="<<label<<", Y[k]="<<Y[k]<<", f="<<f<<", g="<<g<<", alpha="<<alpha<<", move="<<move);

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

//#define DO_WEIGHT_NOR
#ifdef DO_WEIGHT_NOR
	//权重归一化

	for_n(i, INPUT_SIZE)
	{
		real square_sum = 0;
		for_n(j, HIDDEN_SIZE)
		{
			square_sum += XH[i][j] * XH[i][j];
		}

		square_sum = sqrt(square_sum);
		for_n(j, HIDDEN_SIZE)
		{
			XH[i][j] /=square_sum;
		}
	}

	for_n(j, HIDDEN_SIZE)
	{
		real square_sum = 0;
		for_n(k, OUTPUT_SIZE)
		{
			square_sum += HY[j][k] * HY[j][k];
		}

		square_sum = sqrt(square_sum);
		for_n(k, OUTPUT_SIZE)
		{
			HY[j][k] /=square_sum;
		}
	}
#endif

//	pos_t p=this->predict(board);
//	log_status("train="<<win<<" predict="<<int((char)p));
}

void NeuralNetwork::train(const string& fp) {
	log_warn("NeuralNetwork start training from file: "<<fp<<" ...");

	uint trained=0;
	timer previous;
	string line;
	ifstream fin(fp.c_str());

	uint cnt=0;
	while (getline(fin, line)) {
		if (cnt>2000) break;
		if (cnt % 10 == 0) {
			double gap=previous.elapsed();
			real avg=get_avg_weight();
			log_status("games="<<cnt
					<<" trained="<<trained
					<<" time="<<gap
					<<" speed="<<(0.001*trained/gap)<<"KB/s"
					<<" avg="<<avg
					<<" std="<<get_weight_square(avg)
					);
		}
		++cnt;
//		log_status(line);
		istringstream iss(line);
		Board finished;
		iss>>finished;
//		log_status(finished);

		Board board;
		int pointer=0;
		while (!board.game_over()) {
			uchar mobility=board.mobility();

			if (mobility==0) {
				board.pass();
			} else {
				Move move=finished.get_history_move(pointer);

				if (board.empty_cnt()<=12) {
//				if (true or board.empty_cnt()>12) {
					++trained;
					train_one_move(board, move);
				}

				board.play(move);
				++pointer;
			}
		}
	}

	fin.close();

	log_warn("NeuralNetwork finished training");
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
		for_n(i, INPUT_SIZE)
		{
			H[j] += X[i] * XH[i][j];
		}
	}

	// H -> Y : 每个输出层节点的值等于隐藏节点向量 点乘 对应的权重向量
	log_debug("H -> Y");

#ifdef TRAIN_MODE_WITH_POS
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
	pos_t pos=POS(best / 8, best % 8);}
	return pos;
#elif defined TRAIN_MODE_WITH_WIN
	uchar bits=0;
	for_n(k, OUTPUT_SIZE)
	{
		Y[k] = 0;
		for_n(j, HIDDEN_SIZE)
		{
			Y[k] += H[j] * HY[j][k];
		}
		real f = sigmoid(Y[k]);
//		log_status("f="<<f);

		uchar label=0;
		if (f >= 0.5) {
			label=1;
		}
		bits |= (label << k);
	}
//	if (bits>1)
//		log_status("bits="<<(uint(bits)));
	bits= bits & 0x7F;

	return board.get_current_turn()==BLACK ? bits : 127 - bits;
#elif defined TRAIN_MODE_WITH_WIN_ONE
	real max_score = INT32_MIN;
	uint best_k;

	for_n(k, OUTPUT_SIZE)
	{
		Y[k] = 0;
		for_n(j, HIDDEN_SIZE)
		{
			Y[k] += H[j] * HY[j][k];
		}
		if (Y[k] > max_score) {
			log_status("change best: score:"<<max_score<<" -> "<<Y[k]<<" best_k:"<<best_k<<" -> "<<k);
			max_score = Y[k];
			best_k = k;
		}
	}

	return board.get_current_turn()==BLACK ? best_k : 128 - best_k;
//	int win= int(best_k) - 64;
//	return win;
#endif
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

real NeuralNetwork::get_avg_weight() const {
	real sum=0;
	for_n(i, INPUT_SIZE)
	{
		for_n(j, HIDDEN_SIZE)
		{
			sum += XH[i][j];
		}
	}
	for_n(j, HIDDEN_SIZE)
	{
		for_n(k, OUTPUT_SIZE)
		{
			sum += HY[j][k];
		}
	}

	return sum / (INPUT_SIZE * HIDDEN_SIZE + HIDDEN_SIZE * OUTPUT_SIZE);
}

real NeuralNetwork::get_weight_square(real avg) const {
	real sum=0;
	for_n(i, INPUT_SIZE)
	{
		for_n(j, HIDDEN_SIZE)
		{
			sum += (XH[i][j] - avg) * (XH[i][j] - avg);
		}
	}
	for_n(j, HIDDEN_SIZE)
	{
		for_n(k, OUTPUT_SIZE)
		{
			sum += (HY[j][k] - avg) * (HY[j][k] - avg);
		}
	}

	return sqrt(sum);
}


void NeuralNetwork::reset() {
	log_debug("reset network");
	//空子
	for_n(i, OUTPUT_SIZE)
	{
		Y[i] = 0;
	}

	//随机初始化权重
	for_n(i, INPUT_SIZE)
	{
		for_n(j, HIDDEN_SIZE)
		{
			XH[i][j] = (rand() / (real) RAND_MAX - 0.5) / HIDDEN_SIZE;
		}
	}
	for_n(j, HIDDEN_SIZE)
	{
		for_n(k, OUTPUT_SIZE)
		{
			//可以初始化为0，这样前期的训练快
//			HY[j][k]=0;
			//也可以随机初始化
			HY[j][k] = (rand() / (real) RAND_MAX - 0.5) / HIDDEN_SIZE;
//			HY[j][k] = (rand() / (real) RAND_MAX - 0.5) / OUTPUT_SIZE;
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
		return 0.001;
	if (x > MAX_EXP)
		return 0.999;
	return expTable[(uint) (((x) + MAX_EXP) * (EXP_TABLE_SIZE / MAX_EXP / 2))];
}

void NeuralNetwork::read_input(const Board& board) {
	for_n(i, 64) {
		color c=board.get_stone_color(POS(i/8, i%8));
		uint& b=X[i];
		uint& w=X[i+64];
		uint& e=X[i+64*2];
		uint& a=X[i+64*3];
		if (c==BLACK) {
			b=1;
			w=e=a=0;
		} else if (c==WHITE) {
			b=e=a=0;
			w=1;
		} else if (c==ACTIVE) {
			b=w=0;
			a=e=1;
		} else if (c==EMPTY) {
			b=w=a=0;
			e=1;
		} else {
			assert(false);
		}
	}
}

