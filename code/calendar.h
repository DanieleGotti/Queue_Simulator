/*******************************************************
		     CALENDAR H
*******************************************************/


#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "simulator.h"
#include "event.h"

class calendar{

	event* 	head;
	event*	last;

	public:

	calendar();
	~calendar();
	event* 	get();
	void	put(event* New_event);
	};


inline	calendar::calendar(){
	head=NULL;
	last=NULL;
	}
inline	calendar::~calendar(){
	event* temp=head;
	last->next=NULL;
	while(temp!=NULL){
		temp=temp->next;
		delete head;
		head=temp;
		}
	}

#endif

