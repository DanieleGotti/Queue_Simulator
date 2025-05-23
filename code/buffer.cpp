/***************************************************************************
BUFFER.C
***************************************************************************/

// Inclusione del file di intestazione associato alla classe buffer
#include "buffer.h"

// Costruttore della classe buffer
buffer::buffer(){
	head = NULL;         // Puntatore alla testa della coda inizializzato a NULL
	last = NULL;         // Puntatore alla coda (ultimo elemento) inizializzato a NULL
	status = 0;          // Stato del buffer, inizializzato a 0 (ad esempio libero o vuoto)
	tot_delay = 0.0;     // Ritardo totale accumulato, inizializzato a 0
	tot_packs = 0.0;     // Numero totale di pacchetti gestiti, inizializzato a 0
}

// Metodo per inserire un pacchetto nel buffer
void buffer::insert(packet* pack){
	if (head == NULL) {        // Se il buffer è vuoto
		head = pack;          // Il nuovo pacchetto diventa la testa
		last = pack;          // e anche l'ultimo
		last->next = head;    // Si crea un collegamento circolare (solo un elemento)
	}
	else {
		last->next = pack;    // Il pacchetto corrente diventa il successore dell'ultimo
		last = pack;          // Il pacchetto appena inserito diventa il nuovo ultimo
		last->next = head;    // Si ristabilisce il collegamento circolare
	}
}

// Metodo per rimuovere e restituire un pacchetto dalla testa del buffer
packet* buffer::get(){
	packet* pack;

	if (head == NULL)          // Se il buffer è vuoto
		return NULL;           // Non c'è nulla da restituire

	if (last == head) {        // Se c'è un solo pacchetto nel buffer
		pack = head;           // Lo si salva per il ritorno
		last = NULL;           // Il buffer torna vuoto
		head = NULL;
	}
	else {
		pack = head;           // Salva il pacchetto da rimuovere
		head = head->next;     // Avanza la testa al prossimo pacchetto
		last->next = head;     // Ristabilisce il collegamento circolare
	}

	return pack;               // Restituisce il pacchetto rimosso
}
