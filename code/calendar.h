/*******************************************************
CALENDAR H
*******************************************************/

#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "simulator.h"
#include "event.h"

// Classe calendar:
// Gestisce una lista concatenata di eventi (event) in ordine temporale,
// utilizzata per mantenere il calendario degli eventi nella simulazione.
class calendar {

	event* head;  // Puntatore al primo evento nella lista (evento con tempo minore)
	event* last;  // Puntatore all'ultimo evento nella lista (evento con tempo maggiore)

public:

	calendar();   // Costruttore: inizializza lista vuota
	~calendar();  // Distruttore: libera la memoria degli eventi rimasti

	// Estrae (rimuove) e restituisce il primo evento della lista (evento più prossimo nel tempo)
	event* get();

	// Inserisce un nuovo evento ordinandolo nella lista in base al tempo
	void put(event* New_event);
};


// Costruttore inline: inizializza la lista come vuota
inline calendar::calendar() {
	head = NULL;
	last = NULL;
}

// Distruttore inline: elimina tutti gli eventi ancora presenti nella lista
inline calendar::~calendar() {
	event* temp = head;
	// Disconnette il puntatore next dell'ultimo evento per evitare accessi invalidi
	last->next = NULL;

	// Ciclo di cancellazione di tutti gli eventi nella lista
	while (temp != NULL) {
		temp = temp->next;  // Passa all'evento successivo
		delete head;        // Cancella l'evento corrente
		head = temp;        // Aggiorna head al prossimo evento
	}
}

#endif
