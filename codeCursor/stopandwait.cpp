/*
 * STOPANDWAIT.CPP
 *
 * Implementation of the StopAndWaitSimulator class, which simulates a stop-and-wait queueing system
 * with Poisson arrivals, exponential service, transmission errors, and explicit ACK channel.
 *
 * The simulator collects user parameters, runs multiple simulation runs, and computes statistics
 * on the average packet delay, confidence intervals, and retransmissions.
 */

#include "stopandwait.h"
#include "easyio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Constructor: initializes statistics and counters
StopAndWaitSimulator::StopAndWaitSimulator(int argc, char *argv[]) : simulator(argc, argv) {
    delay = new Sstat();
    total_retransmissions = 0;
}

// Destructor: releases statistics memory
StopAndWaitSimulator::~StopAndWaitSimulator() {
    delete delay;
}

// Reads all simulation parameters from the user
void StopAndWaitSimulator::input() {
    printf("\nMODEL PARAMETERS:\n\n");
    printf("\n Arrivals model:\n");
    printf("1 - Poisson:>\n");
    read_int("", 1, 1, 1);
    double load = read_double("Traffic load (Erlang)", 0.4, 0.01, 0.999);
    printf("\n Service model:\n");
    printf("1 - Exponential:>\n");
    read_int("", 1, 1, 1);
    double avg_service = read_double("Average service duration (s)", 0.4, 0.01, 100);
    mu = 1.0 / avg_service;
    lambda = load * mu;
    p = read_double("Error probability p", 0.01, 0.0, 1.0);
    delta = read_double("ACK arrivals rate delta (1/s)", 1.0, 0.01, 100);
    double avg_ack_service = read_double("ACK average service duration (s)", 0.1, 0.01, 100);
    muack = 1.0 / avg_ack_service;
    printf("\nSIMULATION PARAMETERS:\n\n");
    Trslen = read_double("Simulation transient len (s)", 100, 0.01, 10000);
    Runlen = read_double("Simulation RUN len (s)", 100, 0.01, 10000);
    NRUNmin = read_int("Simulation number of RUNs", 5, 2, 100);
    printf("\n");
}

// Resets counters for a new simulation run
void StopAndWaitSimulator::clear_counters() {
    packets = 0.0;
    tot_delay = 0.0;
    total_retransmissions = 0;
}

// Resets statistics for a new set of runs
void StopAndWaitSimulator::clear_stats() {
    delay->reset();
}

// Adds a sample to the delay statistics
void StopAndWaitSimulator::update_stats(double d) {
    *delay += d;
}

// Prints the trace for a single run
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

// Initializes the simulation (reads parameters)
void StopAndWaitSimulator::init() {
    input();
}

// Main simulation loop: executes NRUNmin runs, each with its own statistics
void StopAndWaitSimulator::run() {
    clear_stats();
    int current_run = 1;
    while (current_run <= NRUNmin) {
        clear_counters();
        double clock = 0.0;
        while (clock < Runlen) {
            // Generate next packet arrival (Poisson process)
            double inter_arrival = -log(1.0 - ((double)rand() / RAND_MAX)) / lambda;
            clock += inter_arrival;
            if (clock >= Runlen) break;
            // Simulate stop-and-wait transmission with possible retransmissions
            double total_delay = 0.0;
            int retransmissions = 0;
            bool error = ((double)rand() / RAND_MAX) < p;
            while (error) {
                // Each failed transmission: add service time, count retransmission
                double service_time = -log(1.0 - ((double)rand() / RAND_MAX)) / mu;
                total_delay += service_time;
                retransmissions++;
                error = ((double)rand() / RAND_MAX) < p;
            }
            // Last (successful) transmission
            double service_time = -log(1.0 - ((double)rand() / RAND_MAX)) / mu;
            total_delay += service_time;
            // Wait for ACK arrival and processing
            double ack_wait = -log(1.0 - ((double)rand() / RAND_MAX)) / delta;
            double ack_proc = -log(1.0 - ((double)rand() / RAND_MAX)) / muack;
            total_delay += ack_wait + ack_proc;
            packets += 1.0;
            tot_delay += total_delay;
            total_retransmissions += retransmissions;
            clock += total_delay;
        }
        if (packets > 0)
            update_stats(tot_delay / packets);
        print_trace(current_run);
        current_run++;
    }
}

// Prints the final results, including statistics and theoretical values
void StopAndWaitSimulator::results() {
    fprintf(fpout, "*********************************************\n");
    fprintf(fpout, "           SIMULATION RESULTS                \n");
    fprintf(fpout, "*********************************************\n\n");
    fprintf(fpout, "Input parameters:\n");
    fprintf(fpout, "Transient length (s)         %5.3f\n", Trslen);
    fprintf(fpout, "Run length (s)               %5.3f\n", Runlen);
    fprintf(fpout, "Number of runs               %5d\n", NRUNmin);
    fprintf(fpout, "Traffic load                 %5.3f\n", lambda / mu);
    fprintf(fpout, "Average service duration     %5.3f\n", 1.0 / mu);
    fprintf(fpout, "Error probability            %5.3f\n", p);
    fprintf(fpout, "ACK arrivals rate            %5.3f\n", delta);
    fprintf(fpout, "ACK average service duration %5.3f\n", 1.0 / muack);
    fprintf(fpout, "Results:\n");
    fprintf(fpout, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
            delay->mean(),
            delay->confidence(.95),
            delay->confpercerr(.95));
    // Theoretical delay: tT/(1-p), where tT = 1/mu + 1/delta + 1/muack
    double ttx = 1.0 / mu;
    double tack = 1.0 / delta + 1.0 / muack;
    double tT = ttx + tack;
    double teorico = tT / (1.0 - p);
    fprintf(fpout, "Theoretical average delay    %2.6f\n", teorico);
    fprintf(fpout, "Difference (sim - theory)    %+.6f\n", delay->mean() - teorico);
    fprintf(fpout, "Total retransmitted packets  %ld\n", total_retransmissions);
    fprintf(fpout, "D  %2.6f   %2.6f   %.2e %2.6f\n\n",
            lambda / mu, delay->mean(), delay->confidence(.95), teorico);
} 