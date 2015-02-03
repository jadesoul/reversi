/**
 * File: neuraltrain.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-02-03 22:12:41.385421
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/engine.h"

class NeuralNetworkTest {
	public:
		NeuralNetwork network;

		NeuralNetworkTest() {
			ofstream init("model/init.model");
			init<<network;
			network.train();
			ofstream trained("model/trained.model");
			trained<<network;
		}
};

int main () {
	openings = new OpeningBook();
	init_board_hash_table();

	NeuralNetworkTest a;

	delete openings;
	return 0;
}
