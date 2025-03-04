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

#define HIDDEN_SIZE 64//隐藏层的节点个数
typedef float real;

#define EXP_TABLE_SIZE 1000			//sigmoid缓存表大小
#define MAX_EXP 6					//sigmoid函数的输入小于-6时，值接近0，大于6时，值接近1，在这个范围内的输入缓存起来，超过范围直接给出取值

class NeuralNetwork {
	private:
//		color X[64];//输入层，对应棋盘上的64个棋子，各个棋子的取值可以是BLACK,WHITE,EMPTY
		const color * X;//指向一个board.map即可

		real XH[64][HIDDEN_SIZE];//输入层到隐藏层的权重矩阵XH[i][j]代表weight(X[i], H[j])
		real H[HIDDEN_SIZE];//隐藏层节点的取值
		real H_ERR[HIDDEN_SIZE];//隐藏层节点的误差
		real HY[HIDDEN_SIZE][64];//输入层到隐藏层的权重矩阵HY[j][k]代表weight(H[j], Y[k])
		real Y[64];//输出层，对应棋盘上64个棋子，在各个位置下子的得分
		real alpha;//学习率
		real* expTable;			//存放预先计算出的sigmoid函数值

	public:
		NeuralNetwork():alpha(0.025) {
			srand((int) time(NULL)); //初始化随机数种子
			reset();
			expTable=new real[EXP_TABLE_SIZE];
			fill_sigmoid_table();
		}

		~NeuralNetwork() {
			delete [] expTable;
		}

		void train() {
			log_warn("NeuralNetwork start training ...");
			if (true) {
				//			RandomAIPlayer black, white;
				//			Game game(black, white);

				//先拿开局库试水
				map<Board, Choices>& book=openings->book;
				uint total=book.size();
				uint cnt=0;
				for (pair<const Board, Choices>& entry : book) {
					++cnt;
					if (cnt%10000==0) log_warn("trained: "<<cnt<<"/"<<total<<"="<<(float(cnt)/total));
					const Board& board=entry.first;
					Choices& choices=entry.second;

					log_debug(board);

					//拷贝棋盘
//					memcpy((void*)X, (const void*)board.map, 64);
					//性能优化
					X=&(board.map[0][0]);

					for (pair<const move_t, double>& choice : choices) {
						move_t move=choice.first;
						Pos pos(move);
						uint kk = pos.x * 8 + pos.y;
						double score=choice.second;

						log_debug("train nwetwork from move:"<<Move(pos, board.turn)<<", score="<<score);

						//将当前棋步作为训练数据
						reset_hidden();

						// X -> H : 每个隐藏层节点的值等于输入层向量 点乘 对应的权重向量
						log_debug("X -> H");
						for_n(j, HIDDEN_SIZE) {
							H[j]=0;
							for_n(i, 64) {
								H[j] += X[i] * XH[i][j];
							}
						}

						// H -> Y : 每个输出层节点的值等于隐藏节点向量 点乘 对应的权重向量
						log_debug("H -> Y");
						for_n(k, 64) {

							real label;

							if (k==kk) {//正样本
								label=1;
							} else {//负样本
								if ((rand()/(real)RAND_MAX)<0.01)
									label=0;	
								else
									continue;
							}

							Y[k]=0;
							for_n(j, HIDDEN_SIZE) {
								Y[k] += H[j] * HY[j][k];
							}

							//计算梯度
							real f=sigmoid(Y[k]);
							double g = alpha * (label - f);
							log_debug("caculate label="<<label<<", f="<<f<<", g="<<g<<", alpha="<<alpha<<", move="<<Pos(move));

							//Y -> H: 计算误差向量,后向传播误差
							log_debug("Y -> H");
							for_n(j, HIDDEN_SIZE) {
								H_ERR[j] += g * HY[j][k];	
							}

							//更新隐藏层到输出层权重
							for_n(j, HIDDEN_SIZE) {
								HY[j][k] += g * H[j];
							}

							//H -> X: 误差向量累加到输入到隐藏层权重向量 
							log_debug("H -> X");
							for_n(i, 64) {
								for_n(j, HIDDEN_SIZE) {
									XH[i][j] += H_ERR[j];
								}
							}
						}
					}
				}
			}
			log_warn("NeuralNetwork finished training");
		}

	move_t predict(const Board& board) {
		X = &(board.map[0][0]);

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

		real max_score=INT32_MIN;
		uint best;

		for_n(k, 64)
		{
			uint x=k/8, y=k%8;
			if (! board.is_active(x, y)) continue;

//			move_t move=Pos(k/8, k%8).tomove();
			Y[k] = 0;
			for_n(j, HIDDEN_SIZE)
			{
				Y[k] += H[j] * HY[j][k];
			}
			if (Y[k]>max_score) {
				max_score=Y[k];
				best=k;
			}
		}
		return Pos(best/8, best%8).tomove();
	}
	
	ostream& dump(ostream& o=cout) const {
		o<<"===================NeuralNetwork==================="<<endl;
		o<<"XH[j][i]:"<<endl;
		for_n(i, 64) {
			for_n(j, HIDDEN_SIZE) {
				o<<XH[i][j]<<' ';
			}
			o<<endl;
		}
		o<<"HY[j][k]:"<<endl;
		for_n(j, HIDDEN_SIZE) {
			for_n(k, 64) {
				o<<HY[j][k]<<' ';
			}
			o<<endl;
		}
		return o<<endl;
	}
	friend inline ostream& operator<<(ostream& o, const NeuralNetwork& n) { return n.dump(o); }
	

	private:
		void reset() {
			log_debug("reset network");
			//空子
			for_n(i, 64) {
//				X[i]=EMPTY;
				Y[i]=0;
			}

			//随机初始化权重
			for_n(i, 64) for_n(j, HIDDEN_SIZE) {
				XH[i][j]=(rand() / (real)RAND_MAX - 0.5) / HIDDEN_SIZE;
				//可以初始化为0，这样前期的训练快
				//HY[j][i]=0;//(rand() / (real)RAND_MAX - 0.5) / HIDDEN_SIZE;
				//也可以随机初始化
				HY[j][i]=(rand() / (real)RAND_MAX - 0.5) / HIDDEN_SIZE;
			}

			reset_hidden();
		}

		void reset_hidden() {
			log_debug("reset hidden node values");
			for_n(i, HIDDEN_SIZE) {
				H[i]=0;
				H_ERR[i]=0;
			}
		}

		void fill_sigmoid_table() {
			log_info("fill sigmoid table");
			//sigmoid(x)= 1/(1+exp(-x)) = exp(x)/(1+exp(x))
			for (int i = 0; i < EXP_TABLE_SIZE; i++) {
				expTable[i] = exp((i / (real)EXP_TABLE_SIZE * 2 - 1) * MAX_EXP); // Precompute the exp() table
				expTable[i] = expTable[i] / (expTable[i] + 1);                   // Precompute f(x) = x / (x + 1)


			}

		}
	
	inline real sigmoid(real x) {
		if (x<-MAX_EXP) return 0;
		else if (x>MAX_EXP) return 1;
		return expTable[(uint)(((x) + MAX_EXP) * (EXP_TABLE_SIZE / MAX_EXP / 2))];
	}
};

class NeuralNetworkTest {
	public:
		NeuralNetwork network;

		NeuralNetworkTest() {
			ofstream init("init.model");
			init<<network;
			network.train();
			ofstream trained("trained.model");
			trained<<network;
		}
};

class NeuralNetworkAIPlayer : public Player {
private:
	NeuralNetwork network;

public:
	NeuralNetworkAIPlayer() {

		log_warn("start generating big book, size="<<openings->book.size());
		RandomAIPlayer player;//用于推演时随机下棋
		//利用蒙特卡罗思想，开始随机下棋若干次
		uint total=10000;
		for_n(i, total) {
			if (i%1000==0) log_warn("generated: "<<i<<"/"<<total<<"="<<(float(i)/total));
			Game game(player, player);//举办一场比赛
			game.start_expand_opening(openings->book);
		}
		log_warn("generated big book, size="<<openings->book.size());

		network.train();
	}

	uchar play(Board& b) {
		uchar self=b.turn;

//		move_t opening_move=openings->lookup(b);
//		if (opening_move!=PASS) {
//			assert(b.is_active(opening_move));
//			b.play(opening_move);
//			return opening_move;
//		}

		move_t best_move=network.predict(b);

		assert(best_move!=PASS);
		uint x=best_move>>4, y=best_move&0x0F;
		log_info(((self==BLACK)?"BLACK":"WHITE")<<" NeuralNetworkAIPlayer, play at "<<Pos(x, y));
		b.play(x, y);
		return best_move;
	}
};


#endif /* NEURALNET_H_1421152572_56 */
