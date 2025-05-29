/* -*- C++ -*- */
/********************************************************
		SIMULATOR.H
	Abstract class for simulation program

*******************************************************/

#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <stdio.h>

class simulator{
protected:
      FILE *fptrc;
	FILE *fpout;
	void read_args(int argc,char* argv[]);
	virtual void input(void) = 0;	
	virtual void print_trace(int i) = 0;
public:
	simulator(int argc,char* argv[]);
	virtual ~simulator(void) {;}
	virtual void init(void) = 0;
	virtual void run(void) = 0;
	virtual void results(void) = 0;
};


#endif
