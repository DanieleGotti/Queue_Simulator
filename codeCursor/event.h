/*******************************************************
EVENT . H
*******************************************************/

#ifndef _EVENT_H
#define _EVENT_H

#include "global.h"
#include "buffer.h"

// Classe base per eventi nella simulazione
class event {
public:
	event*	next;   // puntatore all'evento successivo nella lista eventi
	double 	time;   // tempo in cui si verifica l'evento

	event();                 // costruttore di default
	event(double Time);      // costruttore con tempo specificato
	event(event* Next, double Time); // costruttore con evento successivo e tempo
	~event(){}               // distruttore vuoto (virtuale per eredità)

	// Metodo virtuale da sovrascrivere per definire il comportamento dell'evento
	virtual void body() {}
};

// Implementazioni inline dei costruttori della classe event
inline event::event() {
	next = NULL;
	time = -1;               // tempo di default -1 indica evento non inizializzato
}

inline event::event(event* Next, double Time) {
	next = Next;
	time = Time;
}

inline event::event(double Time) {
	time = Time;
}

// Classe derivata per eventi di arrivo
class arrival : public event {
	buffer* buf;              // buffer associato all'arrivo (dove inserire il pacchetto)

public:
	int source_id;            // id della sorgente del pacchetto/evento
	virtual void body();      // implementazione del comportamento specifico di arrivo
	arrival(double Time, buffer* Buf); // costruttore con tempo e buffer
};

// Classe derivata per eventi di servizio (servizio del buffer)
class service : public event {
	buffer* buf;              // buffer associato al servizio

public:
	virtual void body();      // implementazione del comportamento specifico di servizio
	service(double Time, buffer* Buf) : event(Time) { buf = Buf; }  // costruttore con tempo e buffer
};

// Implementazione inline del costruttore di arrival
inline arrival::arrival(double Time, buffer* Buf) : event(Time) {
	buf = Buf;
}

#endif
