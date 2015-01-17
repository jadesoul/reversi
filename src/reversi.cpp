/**
 * File: reversi.cpp
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2013-06-18 22:01:41.820000
 * Written In: Peking University, beijing, China
 */

#define APP_NAME "Reversi"
#include "neuralnet.h"
#include "board_test.h"
#include "game_test.h"

extern uint verbose=4;
OpeningBook* openings;

int main () {
	openings=new OpeningBook();

//	 delete new BoardTest;
	delete new GameTest;
	
//	NeuralNetworkTest a;

	// system("pause");

	delete openings;
	return 0;
}
