/* -*- C++ -*- */
#include <math.h>
#include "rand.h"

int 	lseed[11] = {
	0,
	428956419,
	1954324947,
	1145661099,
	1835732737,
	794161987,
	1329531353,
	200496737,
	633816299,
	1410143363,
	1282538739
	};

int arrotonda(double x)
{
double up;

up=ceil(x);
if((up-x)<0.5)
	return (int)up;
else 
	return (int)(up-1);
}

int transmit(double g)
{
double prob;
PSEUDO(SEED,prob);
if(prob<g)
	return 1;
else
	return 0;
}


int fact(int i)
{
if(i<=1)
	return 1;
else
	return i*fact(i-1);
}

int n_interf_by_poisson(double average)
{
double y,out;
int x;

PSEUDO(SEED,y);
for(x=1;;x++)
	{
	out=exp(-average)*pow(average,x*1.0)/fact(x);
	if(y<=out)
		break;
	}
return x;
}
