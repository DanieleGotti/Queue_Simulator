/***************************************************************************
PACKET.H
***************************************************************************/

#ifndef PACKET_H
#define PACKET_H

#include "global.h"  // Include definizioni globali e costanti comuni

// Classe packet: rappresenta un pacchetto con un tempo di generazione
class packet {

	double gen_time;  // Tempo in cui il pacchetto è stato generato

public:
	packet(double Gen_time);  // Costruttore che imposta il tempo di generazione
	~packet(){}               // Distruttore vuoto (non fa nulla di particolare)

	packet* next;  // Puntatore al prossimo pacchetto (per liste collegate)

public:
	double get_time(){ return gen_time; }  // Metodo per ottenere il tempo di generazione
};

// Implementazione inline del costruttore
inline packet::packet(double Gen_time){
	gen_time = Gen_time;  // Inizializza il tempo di generazione
	next = NULL;          // Inizializza il puntatore al prossimo pacchetto a NULL
}

#endif
