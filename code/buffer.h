/***************************************************************************
BUFFER.H
***************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#include "packet.h"  // Include la definizione della classe packet

// Classe buffer:
// Gestisce una lista di pacchetti (packet) in FIFO tramite due puntatori:
// head -> primo pacchetto della lista
// last -> ultimo pacchetto inserito nella lista
class buffer {

	packet* head;  // Puntatore al primo pacchetto nella coda
	packet* last;  // Puntatore all'ultimo pacchetto nella coda

public:
	int status;   // Stato generico del buffer, utilizzabile esternamente

public:
	buffer();    // Costruttore: inizializza il buffer
	~buffer(){}  // Distruttore vuoto (da implementare se necessario)

	// Inserisce un pacchetto alla fine della coda
	void insert(packet* pack);

	// Restituisce (e rimuove) il primo pacchetto dalla coda
	packet* get();

	// Restituisce il puntatore al primo pacchetto (head) senza rimuoverlo
	packet* full() { return head; }

	// Statistiche del buffer
	double tot_delay;   // Totale ritardo accumulato dei pacchetti
	double tot_packs;   // Numero totale di pacchetti processati
};

#endif
