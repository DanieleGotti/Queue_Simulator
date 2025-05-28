/* -*- C++ -*- */
/*******************************************************
		CALENDAR C
*******************************************************/

// Inclusione dei file di intestazione globali e della classe calendar
#include "global.h"
#include "calendar.h"

// Metodo per ottenere il prossimo evento (rimozione dalla testa della lista)
event* calendar::get() {
	if (head == NULL)               // Se il calendario è vuoto
		return NULL;               // Non c'è evento da restituire

	event* ev;

	if (head == last) {            // Se c'è un solo evento nella lista
		ev = head;                // Salva l'evento da restituire
		head = NULL;              // Ripristina lista vuota
		last = NULL;
		return ev;                // Restituisce l'evento
	}

	ev = head;                     // Salva l'evento corrente
	head = head->next;            // Avanza la testa
	last->next = head;            // Mantiene la struttura circolare
	return ev;                    // Restituisce l'evento
}

// Metodo per inserire un nuovo evento mantenendo l'ordine temporale
void calendar::put(event* New) {
	event* temp = head;           // Puntatore temporaneo per scorrere la lista
	event* pippo;                 // Puntatore ausiliario (inutile qui, ma inizializzato)
	pippo = New;

	if (head == NULL) {           // Inserimento nella lista vuota
		head = New;
		head->next = New;        // Lista circolare con un solo elemento
		last = New;
	}
	else if (New->time < head->time) {  // Inserimento in testa
		New->next = head;
		head = New;
		last->next = head;       // Aggiorna la circolarità
	}
	else if (last == head) {      // Se c'è un solo evento nella lista
		if (New->time < head->time) {
			head = New;
			head->next = last;
			last->next = head;
		}
		else {
			last = New;
			head->next = last;
			last->next = head;
		}
	}
	else if (last->time < New->time) {  // Inserimento in fondo
		last->next = New;
		last = New;
		last->next = head;         // Chiude la circolarità
	}
	else {                         // Inserimento in posizione intermedia
		while (temp->next->time < New->time)
			temp = temp->next;    // Cerca la posizione corretta
		New->next = temp->next;   // Inserisce l'evento
		temp->next = New;
	}
}
