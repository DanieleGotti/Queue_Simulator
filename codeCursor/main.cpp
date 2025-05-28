/* -*- C++ -*- */
#include "stdio.h"
#include "global.h"
#include "queue.h"
#include "simulator.h"
#include "stopandwait.h"
#include "easyio.h"

// Funzione principale: punto di ingresso del programma
int main(int argc, char *argv[])
{
	simulator *eval = nullptr;

	// Pulizia visiva della console con molte righe vuote
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	// Intestazione di benvenuto
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

	// Inizializzazione della simulazione (lettura parametri, setup iniziale)
	eval->init();

	// Avvio dell'esecuzione della simulazione (gestione eventi)
	eval->run();

	// Calcolo e visualizzazione dei risultati finali
	eval->results();

	// Deallocazione dell'oggetto simulatore per liberare memoria
	delete(eval);
	return 0;
}
