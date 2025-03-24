/*
 * File: reversi.c
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年3月10日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "search.h"

void test_game() {
	init_board();
	start_game(1, 1);
}

void test_opening() {
	init_board();
	make_move(C4);//black make first move can be: F5, E6, D3, C4
	make_move(C3);
	make_move(D3);
	make_move(C5);
	make_move(B3);
	make_move(F4);
	make_move(B5);
	make_move(B4);
	make_move(C6);
	make_move(D6);
	make_move(F5);
	make_move(F6); // candiates: b2, d2, b6, f6
	fast_play(32);
	print_board();

	int win=mtd(0, empty_cnt);
	printf("win=%d \n", win);
}

void bench_mark(int verbose, int randplay, int parallel) {//TODO: add parallel mode to speed up by multi-core CPU processors (using fork)
	int n = 1000000;
	int total_win=0;
	int total_full=0;

	for (int i=0; i<n; i++) {
		if (verbose && i%1000==0) printf("benchmark: %d / %d \n", i, n);
		init_board();

		// try some opennings
		// C4c3                                     Diagonal Opening 
		// C4c3D3c5B2                               X-square Opening (t3) 
		// C4c3D3c5B3                               Snake, Peasant (t3) 
		// C4c3D3c5B3f4B5b4C6d6F5                   Pyramid, Checkerboarding Peasant (t3) 

		make_move(C4);//black make first move can be: F5, E6, D3, C4
		make_move(C3);
		make_move(D3);
		make_move(C5);
		make_move(B3);
		make_move(F4);
		make_move(B5);
		make_move(B4);
		make_move(C6);
		make_move(D6);
		make_move(F5);

		{
			// make_move(B2); // -0.15
			// make_move(D2); // -0.5
			// make_move(B6); // -0.12
			make_move(F6); // -0.8
		}

		{
			// make_move(A4); // -0.9
			// make_move(E6); // -0.3
			// make_move(G6); // -0.34
			// make_move(G7); // -2
		}

		fast_play(20);

		// fork(); // TODO: it is hard to assign tasks to parallel sub-process



		total_win += start_game(0, randplay);
		if (em == 0) total_full++;
	}
	printf("stats: %d / %d = %f; total_full=%d \n", total_win, n, 1.0*total_win/n, total_full);
}

void test_undo() {
	print_board();

	search();
	print_board();
	search();
	print_board();

	printf("now undo \n");
	undo_move(sequence[played_cnt-1]);
	print_board();

	printf("now undo \n");
	undo_move(sequence[played_cnt-1]);
	print_board();

	search();
	print_board();
}

void test_search() {
	for_n(j, 60) {
		init_board();
		TIMER_START(now);
		int i=j+1;
		if (rand_play(60-i)) {
			int win;

			print_board();

			win=mtd(0, i);
	//		win=negamax(i, 0);
	//		win=alpha_beta(-64, 64, i, 0);
//			win=pvs(-64, 64, i, 0);

			printf("%d levels, using %f seconds, win=%d \n", i, TIMER_ELASPED(now), (turn==BLACK?win:-win));
		}
	}
}

void test_depening() {
	printf("%d\n", deepening(6000));
}

int test() {
	int m;
	get_mobility(m);
	printf("%d\n", m);
	printf("%d\n", start_game(1, 0));

	return 0;
}


int fork_demo() { // 用fork不行，不太好和子进程交互数据，分配任务
    pid_t pid;

    // 调用 fork() 创建子进程
    pid = fork();

    if (pid < 0) {
        // fork() 失败
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // 子进程
        printf("This is the child process. PID: %d\n", getpid());
    } else {
        // 父进程
        printf("This is the parent process. PID: %d, Child PID: %d\n", getpid(), pid);
		wait(NULL);
    }

    return 0;
}


void* thread_function(void* arg) {
    int* value = (int*)arg;
    printf("子线程收到值: %d\n", *value);

    // 修改值并返回
    *value *= 2;
    pthread_exit((void*)value);
}

int pthread_demo() {
    pthread_t thread_id;
    int value = 1;

    // 创建子线程
    if (pthread_create(&thread_id, NULL, thread_function, (void*)&value) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // 等待子线程结束并获取返回值
    void* thread_return;
    if (pthread_join(thread_id, &thread_return) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    int* result = (int*)thread_return;
    printf("主线程收到子线程返回的值: %d\n", *result);

    return 0;
}

void benchmark_parallel() {
	// parallel need the board should not be global variable, but can be copy to multiple parts to be handle by multiple thread
	// need revamp the board
}

int main() {
	srand((uint) time(NULL)); // init random seed

	// printf("-1 %% 64 = %d\n", ((uint)(-1) % 64));

	init_board();

	// init_valid_move_byte_table();
	// test_undo();
	test_depening();

	// test_game();
	// test_search();
	// test_opening();

	// bench_mark(0, 0, 1);

	return 0;
}
