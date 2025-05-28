/* -*- C++ -*- */
/********************************************************
		SIMULATOR.H
	Classe astratta per programmi di simulazione

*******************************************************/

#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <stdio.h>

class simulator {
protected:
	FILE *fptrc;   // file pointer per il trace (log di eventi)
	FILE *fpout;   // file pointer per l'output dei risultati

	void read_args(int argc, char* argv[]);  // metodo per leggere argomenti da riga di comando

	// Metodi astratti che devono essere implementati nelle classi derivate
	virtual void input(void) = 0;         // metodo per leggere input specifico della simulazione
	virtual void print_trace(int i) = 0; // metodo per stampare il trace/log

public:
	simulator(int argc, char* argv[]);  // costruttore con argomenti da linea di comando
	virtual ~simulator(void) {;}        // distruttore virtuale (vuoto)

	// Metodi astratti pubblici che definiscono il ciclo della simulazione
	virtual void init(void) = 0;    // inizializza la simulazione
	virtual void run(void) = 0;     // esegue la simulazione
	virtual void results(void) = 0; // mostra i risultati
};

#endif
