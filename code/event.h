/*******************************************************
		     EVENT . H
*******************************************************/


#ifndef _EVENT_H
#define _EVENT_H

#include "global.h"
#include "buffer.h"


class event{
public:
	event*	next;	// next event
	double 	time;	// event time
	event();
	event(double Time);
	event(event* Next, double Time);
	~event(){}
	virtual void body(){}
};

inline event::event(){
	next=NULL;
	time=-1;
	}

inline event::event(event* Next, double Time){
	next=Next;
	time=Time;
	}

inline event::event(double Time){
	time=Time;
	}

class arrival: public event{

	buffer* buf;

	public:
	int source_id;
	virtual void body();
	arrival(double Time, buffer* Buf);
	};

class service: public event{

	buffer* buf;

	public:
	virtual void body();
	service(double Time, buffer* Buf): event(Time){buf=Buf;}
	};

inline arrival::arrival(double Time, buffer* Buf): event(Time){
	buf=Buf;
	}

#endif

