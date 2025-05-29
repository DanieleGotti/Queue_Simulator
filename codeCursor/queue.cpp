/* -*- C++ -*- */
/*******************************************************
		QUEUE.C
*******************************************************/
#include "global.h"
#include <stdio.h>
#include "queue.h"
#include "rand.h"
#include "buffer.h"
#include "event.h"
#include "calendar.h"
#include "easyio.h"

calendar* cal;		// events calendar
double	inter;
double 	duration;
double 	Trslen;
double 	Runlen;
int 	NRUNmin;
int 	NRUNmax;


queue::queue(int argc,char *argv[]): simulator(argc,argv)
{
cal=new calendar();
buf = new buffer();
delay=new Sstat();
}

queue::~queue()
{
delete delay;
delete cal;
delete buf;
}

void queue::input(){
printf("MODEL PARAMETERS:\n\n");
	printf("\n Arrivals model:\n");
	printf("1 - Poisson:>\n");
	traffic_model=read_int("",1,1,1);
	load=read_double("Traffic load (Erlang)",0.4,0.01,0.999);
	printf("\n Service model:\n");
	printf("1 - Exponential:>\n");
	service_model=read_int("",1,1,1);
	duration=read_double("Average service duration (s)",0.4,0.01,100);
	inter=duration/load;
printf("SIMULATION PARAMETERS:\n\n");
	Trslen=read_double("Simulation transient len (s)", 100, 0.01, 10000);
	Trslen=Trslen;
	Runlen=read_double("Simulation RUN len (s)",  100, 0.01, 10000);
	Runlen=Runlen;
	NRUNmin=read_int("Simulation number of RUNs", 5, 2, 100);
}



void queue::init()
{
input();
event* Ev;
Ev=new arrival(0.0, buf);
cal->put(Ev);
buf->status=0;
}

void queue::run(){
	
	extern double 	Trslen;
	extern double 	Runlen;
	extern int 	NRUNmin;
	extern int 	NRUNmax;

        double clock=0.0;
        event* ev;
        while (clock<Trslen){
        	ev=cal->get();
        	ev->body();
        	clock=ev->time; 
        	delete(ev);     
        	}
	clear_stats();
	clear_counters();
	int current_run_number=1;
	while(current_run_number<=NRUNmin){
		while (clock<(current_run_number*Runlen+Trslen)){
			ev=cal->get();
	                ev->body();
       	         	clock=ev->time;
                	delete(ev);
			}
		update_stats();
		clear_counters();
		print_trace(current_run_number);
		current_run_number++;
		}
	}


void queue::results()
{
	extern double 	Trslen;
	extern double 	Runlen;
	extern int 	NRUNmin;
	extern int 	NRUNmax;

	fprintf(fpout, "*********************************************\n");
	fprintf(fpout, "           SIMULATION RESULTS                \n");
	fprintf(fpout, "*********************************************\n\n");
	fprintf(fpout, "Input parameters:\n");
	fprintf(fpout, "Transient length (s)         %5.3f\n", Trslen);
	fprintf(fpout, "Run length (s)               %5.3f\n", Runlen);
	fprintf(fpout, "Number of runs               %5d\n", NRUNmin);
	fprintf(fpout, "Traffic load                 %5.3f\n", load);
	fprintf(fpout, "Average service duration     %5.3f\n", duration);
	fprintf(fpout, "Results:\n");
	fprintf(fpout, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n", 
			delay->mean(), 
			delay->confidence(.95), 
			delay->confpercerr(.95));
	fprintf(fpout, "D  %2.6f   %2.6f   %.2e %2.6f\n", load, delay->mean(), delay->confidence(.95), duration*(load)/(1-load));
}

void queue::print_trace(int n)
{
      fprintf(fptrc, "*********************************************\n");
      fprintf(fptrc, "                 TRACE RUN %d                \n", n);
      fprintf(fptrc, "*********************************************\n\n");

	
      fprintf(fptrc, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
                        delay->mean(),              
                        delay->confidence(.95),                            
                        delay->confpercerr(.95));
      fflush(fptrc);

}

void queue::clear_counters()
{
	buf->tot_delay=0.0;
	buf->tot_packs=0.0;
}

void queue::clear_stats()
{
	delay->reset();
}
void queue::update_stats()
{
	*delay+=buf->tot_delay/buf->tot_packs;
}

int queue::isconfsatisf(double perc)
{
        return delay->isconfsatisfied(10, .95);
}
