/* -*- C++ -*- */
#include "stdio.h"
#include "global.h"
#include "queue.h"
#include "simulator.h"

// Funzione principale: punto di ingresso del programma
int main(int argc, char *argv[])
{
	simulator *eval; // Puntatore alla classe base simulator

	// Pulizia visiva della console con molte righe vuote
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	// Intestazione di benvenuto
	printf("**********************************************************\n\n");
	printf("               G/G/1 QUEUE SIMULATION PROGRAM\n\n");
	printf("**********************************************************\n\n");

	// Creazione di un oggetto queue (che rappresenta il simulatore di coda)
	// La classe queue estende simulator e usa argc/argv per configurare i parametri
	eval = new queue(argc, argv);

	// Inizializzazione della simulazione (lettura parametri, setup iniziale)
	eval->init();

	// Avvio dell'esecuzione della simulazione (gestione eventi)
	eval->run();

	// Calcolo e visualizzazione dei risultati finali
	eval->results();

	// Deallocazione dell'oggetto simulatore per liberare memoria
	delete(eval);
}
