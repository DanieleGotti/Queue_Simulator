/* -*- C++ -*- */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*		<<<NUOVO>>>	    R A N D . H				*/
/*                                                                      */
/*            MODULO DI GENERAZIONE EVENTI CASUALI                      */
/*                                                                      */
/*     Questo modulo contiene le macro per la generazione di eventi     */
/*     pseudocasuali:                                                   */
/*            - RESET_SEED : prepara i semi della routine PSEUDO        */
/*            - PSEUDO     : genera un numero pseudocasuale compreso    */
/*                           tra 0 e 1                                  */
/*	      - GEN_UNIF   : genera un numero intero compreso tra	*/
/*			     min e max					*/
/*	      - GEN_POISSON: genera un evento di poisson		*/
/*	      - GEN_BERNOULLI: genera un numero con prob. bernoulliana  */
/*	      - GEN_EXP    : genera un esponenziale			*/
/*	      - GEN_EXP_INT: genera un exp e lo approssima con un int	*/ 
/*	      - GEN_EXP_ONE: genera un exp intero di valore minimo 1    */
/*                                                                      */
/*----------------------------------------------------------------------*/
#ifndef _RAND_H
#define _RAND_H
#include <math.h>
#include "global.h"


/* 
 * vettore contenente i semi per la generazione di eventi casuali
 * inizializzato in rand.c oppure inizializzato chiamando RESET_SEED
 */
extern	int	lseed[11];



/*
 *macro di inizializzazione semi 
 */
#define RESET_SEED		\
{				\
lseed[0] = 0;			\
lseed[1] = 428956419;		\
lseed[2] = 1954324947;		\
lseed[3] = 1145661099;		\
lseed[4] = 1835732737;		\
lseed[5] = 794161987;		\
lseed[6] = 1329531353;		\
lseed[7] = 200496737;		\
lseed[8] = 633816299;		\
lseed[9] = 1410143363;		\
lseed[10] =1282538739;		\
}




/*
 * MACRO di generazione casuale di un double compreso tra 0 e 1 
 *
 * come ingresso riceve il valore  IS = seme di generazione (1..10)
 * come uscita assegna il valore casuale alla variabile
 * double PSEUDONUM
 */

#define	la  16807
#define	lb15  32768
#define	lb16  65536
#define	lp  2147483647
#define PSEUDO(IS,PSEUDONUM)                                     \
{                                                                \
int     lfhi,lhi,lalo,leftlo,_k,iseed;				 \
double  _x;							 \
	iseed = lseed [IS];                                      \
	lhi = iseed/lb16;                                        \
	lalo = (iseed-lhi*lb16)*la;                              \
	leftlo = lalo/lb16;                                      \
	lfhi = lhi*la + leftlo;                                  \
	_k = lfhi/lb15;                                          \
	iseed = (((lalo-leftlo*lb16)-lp)+(lfhi-_k*lb15)*lb16)+_k;\
	while (iseed < 0)                                        \
		iseed += lp;                                     \
	lseed[IS] = iseed;                                       \
	_x = lseed[IS];                                           \
	PSEUDONUM=(_x*4.656612875e-10);                           \
}   



/*
 * macro di generazione di un numero intero compreso tra due 
 * limiti (Min e Max)
 */
#define		GEN_UNIF(IS, Min, Max, ESITO)			\
	{							\
	double pseudonum;					\
	PSEUDO((IS), pseudonum);				\
	ESITO = (int)((Min) + pseudonum * (fabs((double)((Max) - (Min)))+1));\
	}




/*
 * macro di generazione di un evento di poisson 
 * 
 * restituisce come valore 0 o 1 a seconda se l'evento e` stato
 * generato o meno. Il valore e` restituito in ESITO.
 * richiede in ingresso il seme di generazione casuale IS e la
 * probabilita` dell'evento PROB
 */

#define 	GEN_POISSON(IS, PROB, ESITO)		\
	{						\
	double pseudonum;				\
	PSEUDO((IS),pseudonum);				\
	if (pseudonum > (PROB)) ESITO=0;		\
	           else ESITO=1;			\
	}



/*
 * macro di generazione di un evento di bernoulli
 *
 * restituisce un numero compreso tra 0 e TOP iterando eventi di
 * poisson
 */

#define		GEN_BERNOULLI(IS, TOP, PROB, ESITO)	\
	{						\
	int NUM;        				\
	int i;						\
	ESITO = 0;					\
	for(i=0; i<TOP; i++)				\
           {						\
	   GEN_POISSON(IS, PROB, NUM);			\
	   ESITO += NUM;				\
	   }						\
	}



/*
 * macro di generazione di un double con distribuzione
 * esponenziale
 */

#define		GEN_EXP(IS, AVERAGE, RES)		\
	{						\
        double pseudonum;				\
        PSEUDO(IS,pseudonum);				\
        RES = -log(pseudonum) * (double)(AVERAGE);	\
  	}



/*
 * macro di generazione di un intero con distribuzione
 * esponenziale
 */

#define 	GEN_EXP_INT(IS, AVERAGE, RES)		\
	{						\
	double expnum;					\
	GEN_EXP(IS, AVERAGE, expnum);			\
	RES = (int)(expnum + 0.5);			\
	if (RES < 0) RES=0;				\
	}

/*
 * macro di generazione di un intero con distribuzione
 * esponenziale e valore minimo 1
 */

#define 	GEN_EXP_ONE(IS, AVERAGE, RES)		\
	{						\
	double expnum;					\
	GEN_EXP(IS, (AVERAGE-0.5), expnum);		\
	RES = (int)(expnum + 1);			\
	if (RES < 0) RES=0;				\
	}

int arrotonda(double x);

int transmit(double g);

int fact(int i);

int n_interf_by_poisson(double average);

#endif	// _MACRO_RANDOM_H
