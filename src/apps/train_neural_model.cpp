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
//		network.save_model("model/init.model");

//		network.train();
//		network.save_model("model/trained.model");

		network.load_model("model/trained.model");
		network.save_model("model/trained2.model");
	}
};

int main () {
	NeuralNetworkTest a;

	return 0;
}

