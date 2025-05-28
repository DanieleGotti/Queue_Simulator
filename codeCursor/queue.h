/*******************************************************
		     G/G/1 QUEUE SIMULATOR
*******************************************************/
#ifndef _QUEUE_H
#define _QUEUE_H

#include "simulator.h"   // Classe base per la simulazione
#include "calendar.h"    // Gestione eventi nel calendario
#include "event.h"       // Definizione degli eventi
#include "buffer.h"      // Gestione del buffer della coda
#include "packet.h"      // Definizione del pacchetto
#include "stat.h"        // Statistiche per le misure

// Classe queue che estende simulator per simulare una coda G/G/1
class queue: public simulator{

	virtual void input(void);     // Metodo per gestire input specifico

	buffer* buf;                  // Buffer della coda (lista pacchetti)
	int traffic_model;            // Modello del traffico (es. distribuzione arrivi)
	double load;                  // Carico della coda (intensità traffico)
	int service_model;            // Modello di servizio (es. distribuzione tempi di servizio)

	// Contatori per la simulazione
	double packets;               // Numero di pacchetti processati
	double tot_delay;             // Ritardo totale accumulato

	// Statistiche sul ritardo
	Sstat* delay;                 // Statistiche per i ritardi dei pacchetti

public:
	queue(int argc,char *argv[]); // Costruttore con argomenti da linea di comando
	virtual ~queue(void);         // Distruttore virtuale

	virtual void init(void);      // Inizializza la simulazione
	virtual void run(void);       // Esegue la simulazione

private:
	virtual void clear_counters(void);   // Pulisce i contatori prima di una nuova simulazione
	virtual void clear_stats(void);      // Pulisce le statistiche raccolte
	virtual void update_stats(void);     // Aggiorna le statistiche dopo un evento
	virtual void print_trace(int Run);   // Stampa il tracciato/log della simulazione per un run specifico
	virtual void results(void);           // Mostra i risultati finali della simulazione
	virtual int isconfsatisf(double perc); // Controlla se la confidenza statistica è soddisfatta (perc = percentuale)
};

#endif
