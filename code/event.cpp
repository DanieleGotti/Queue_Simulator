/***********************************************************************
		EVENT.C
***********************************************************************/

#include "event.h"
#include "buffer.h"
#include "calendar.h"
#include "rand.h"

extern calendar* cal;
extern double inter;
extern double duration;

void arrival::body(){
        event* ev;
	
	// generation of next arrival
	double esito;
	GEN_EXP(SEED, inter, esito);
	ev=new arrival(time+esito, buf);
	cal->put(ev);

	// insert the new packet in the queue
      packet* pack=new packet(time);
	// if some packet is already in the buffer, just insert the new one 
      if(buf->full()||buf->status){
		buf->insert(pack);
		}
	// otherwise let the packet get in the service
	else	{
		buf->tot_packs+=1.0;
		delete pack;
		GEN_EXP(SEED, duration, esito);
		ev=new service(time+esito, buf);
		cal->put(ev);
		buf->status=1;
		}
        }

void service::body(){
	// printf("ingresso             %f\n", time);
	packet* pack;
	pack=buf->get();
	event* ev;
	double esito;
	GEN_EXP(SEED, duration, esito);
	if(pack!=NULL){
		ev=new service(time+esito, buf);
		cal->put(ev);
		buf->tot_delay+=time-pack->get_time();
		// printf("%3.5f    %3.5f\n", time, time-pack->get_time());
		buf->tot_packs+=1.0;
		delete pack;
		}
	else
		buf->status=0;
	}

