/* -*- C++ -*- */
#include "stdio.h"
#include "global.h"
#include "queue.h"
#include "simulator.h"
#include "stopandwait.h"
#include "easyio.h"

int main(int argc, char *argv[])
{
	simulator *eval = nullptr;

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	printf("**********************************************************\n\n");
	printf("               G/G/1 QUEUE SIMULATION PROGRAM\n\n");
	printf("**********************************************************\n\n");
	printf("Select simulation mode:\n");
	printf("1 - Simple queue simulator\n");
	printf("2 - Stop-and-wait simulator\n");
	int mode = read_int("Mode", 1, 1, 2);
	if (mode == 1) {
		eval = new queue(argc, argv);
	} else {
		eval = new StopAndWaitSimulator(argc, argv);
	}

	eval->init();
	eval->run();
	eval->results();
	delete(eval);
	return 0;
}
