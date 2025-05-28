#include "stopandwait.h"
#include "easyio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

StopAndWaitSimulator::StopAndWaitSimulator(int argc, char *argv[]) : simulator(argc, argv) {
    delay = new Sstat();
    total_retransmissions = 0;
}

StopAndWaitSimulator::~StopAndWaitSimulator() {
    delete delay;
}

void StopAndWaitSimulator::input() {
    printf("MODEL PARAMETERS:\n\n");

    // Modello arrivi (solo Poisson, valore fisso 1)
    printf("\n Arrivals model:\n");
    printf("1 - Poisson:>\n");
    read_int("", 1, 1, 1); // Solo per coerenza grafica

    // Carico traffico (utilizzo della coda, Erlang)
    lambda = read_double("Traffic load (Erlang)", 0.4, 0.01, 0.999);

    // Modello servizio (solo esponenziale, valore fisso 1)
    printf("\n Service model:\n");
    printf("1 - Exponential:>\n");
    read_int("", 1, 1, 1); // Solo per coerenza grafica

    // Durata media del servizio in secondi
    double avg_service = read_double("Average service duration (s)", 0.4, 0.01, 100);
    mu = 1.0 / avg_service;

    // Parametri specifici stop-and-wait
    p = read_double("Error probability (p)", 0.01, 0.0, 1.0);
    delta = read_double("ACK arrivals rate (delta)", 1.0, 0.01, 100);
    muack = read_double("ACK service mean duration (muack)", 0.1, 0.01, 100);

    printf("SIMULATION PARAMETERS:\n\n");
    Trslen = read_double("Simulation transient len (s)", 100, 0.01, 10000);
    Runlen = read_double("Simulation RUN len (s)", 100, 0.01, 10000);
    NRUNmin = read_int("Simulation number of RUNs", 5, 2, 100);
}

void StopAndWaitSimulator::clear_counters() {
    packets = 0.0;
    tot_delay = 0.0;
    total_retransmissions = 0;
}

void StopAndWaitSimulator::clear_stats() {
    delay->reset();
}

void StopAndWaitSimulator::update_stats(double d) {
    *delay += d;
}

void StopAndWaitSimulator::print_trace(int Run) {
    fprintf(fptrc, "*********************************************\n");
    fprintf(fptrc, "                 TRACE RUN %d                \n", Run);
    fprintf(fptrc, "*********************************************\n\n");
    fprintf(fptrc, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
            delay->mean(),
            delay->confidence(.95),
            delay->confpercerr(.95));
    fflush(fptrc);
}

void StopAndWaitSimulator::init() {
    input();
}

void StopAndWaitSimulator::run() {
    // Simulazione stop-and-wait: per ogni pacchetto, simula trasmissione, errore, attesa ACK
    clear_stats();
    int current_run = 1;
    while (current_run <= NRUNmin) {
        clear_counters();
        double clock = 0.0;
        long run_retransmissions = 0;
        while (clock < Runlen) {
            // Genera tempo di arrivo pacchetto
            double inter_arrival = -log(1.0 - ((double)rand() / RAND_MAX)) / lambda;
            clock += inter_arrival;
            if (clock >= Runlen) break;
            // Simula trasmissione pacchetto
            double service_time = -log(1.0 - ((double)rand() / RAND_MAX)) / mu;
            double tx_end = clock + service_time;
            // Simula errore
            bool error = ((double)rand() / RAND_MAX) < p;
            double ack_time = 0.0;
            int retransmissions = 0;
            double tx_start = clock;
            while (error) {
                // Ritrasmetti: attesa ACK
                double ack_wait = -log(1.0 - ((double)rand() / RAND_MAX)) / delta;
                ack_time += ack_wait;
                retransmissions++;
                // Nuovo errore?
                error = ((double)rand() / RAND_MAX) < p;
            }
            total_retransmissions += retransmissions;
            // ACK ricevuto
            double ack_wait = -log(1.0 - ((double)rand() / RAND_MAX)) / delta;
            ack_time += ack_wait;
            double total_delay = (tx_end - tx_start) + ack_time;
            packets += 1.0;
            tot_delay += total_delay;
            clock = tx_end + ack_time; // Avanza clock dopo ACK
        }
        if (packets > 0)
            update_stats(tot_delay / packets);
        print_trace(current_run);
        current_run++;
    }
}

void StopAndWaitSimulator::results() {
    fprintf(fpout, "*********************************************\n");
    fprintf(fpout, "           SIMULATION RESULTS                \n");
    fprintf(fpout, "*********************************************\n\n");

    fprintf(fpout, "Input parameters:\n");
    fprintf(fpout, "Transient length (s)         %5.3f\n", Trslen);
    fprintf(fpout, "Run length (s)               %5.3f\n", Runlen);
    fprintf(fpout, "Number of runs               %5d\n", NRUNmin);
    fprintf(fpout, "Traffic load                 %5.3f\n", lambda * (1.0 / mu));
    fprintf(fpout, "Average service duration     %5.3f\n", 1.0 / mu);
    fprintf(fpout, "Error probability (p)        %5.3f\n", p);
    fprintf(fpout, "ACK arrivals rate (delta)    %5.3f\n", delta);
    fprintf(fpout, "ACK service mean duration    %5.3f\n", muack);

    fprintf(fpout, "Results:\n");
    fprintf(fpout, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
            delay->mean(),
            delay->confidence(.95),
            delay->confpercerr(.95));

    // Calcolo teorico tempo medio di attraversamento per stop-and-wait
    // T = (1/(mu*(1-p))) + (1/delta)  (approssimazione: trasmissione + attesa ack, con ritrasmissioni)
    double teorico = (1.0 / (mu * (1.0 - p))) + (1.0 / delta);
    fprintf(fpout, "Theoretical average delay    %2.6f\n", teorico);
    fprintf(fpout, "Difference (sim - theory)    %+.6f\n", delay->mean() - teorico);
    fprintf(fpout, "Total retransmitted packets  %ld\n", total_retransmissions);
} 