#ifndef _STOPANDWAIT_H
#define _STOPANDWAIT_H

#include "simulator.h"
#include "stat.h"

class StopAndWaitSimulator : public simulator {
public:
    StopAndWaitSimulator(int argc, char *argv[]);
    virtual ~StopAndWaitSimulator();

    virtual void init();
    virtual void run();
    virtual void results();

private:
    // Parametri di simulazione
    double lambda;    // Tasso arrivi Poisson
    double mu;        // Tasso servizio (media 1/mu)
    double p;         // Probabilità di errore
    double delta;     // Tasso arrivo ACK (Poisson)
    double muack;     // Media servizio ACK (non usata direttamente, ma richiesta)
    double Trslen;    // Transiente
    double Runlen;    // Durata run
    int NRUNmin;      // Numero run

    // Statistiche
    Sstat* delay;     // Statistiche ritardo

    // Metodi di input e gestione
    void input();
    void clear_counters();
    void clear_stats();
    void update_stats(double d);
    void print_trace(int Run);

    // Variabili di stato
    double packets;
    double tot_delay;
    long total_retransmissions; // Numero totale di pacchetti rinviati (ritrasmissioni)
};

#endif // _STOPANDWAIT_H 