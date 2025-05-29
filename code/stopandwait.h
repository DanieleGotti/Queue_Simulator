/*
 * STOPANDWAIT.H
 *
 * Definition of the StopAndWaitSimulator class, which implements a stop-and-wait queueing system
 * with Poisson arrivals, exponential service, transmission errors, and explicit ACK channel.
 *
 * The simulator allows configuration of traffic load, service time, error probability, ACK arrival rate,
 * and ACK service time, and computes the average packet delay and confidence intervals.
 */

#ifndef _STOPANDWAIT_H
#define _STOPANDWAIT_H

#include "simulator.h"
#include "stat.h"

// StopAndWaitSimulator: simulates a stop-and-wait system with retransmissions and explicit ACKs
class StopAndWaitSimulator : public simulator {
public:
    // Constructor and destructor
    StopAndWaitSimulator(int argc, char *argv[]);
    virtual ~StopAndWaitSimulator();

    // Main simulation interface
    virtual void init();    // Read parameters and initialize simulation
    virtual void run();     // Run the simulation
    virtual void results(); // Print final results

private:
    // Simulation parameters
    double lambda;   // Arrival rate (packets/s)
    double mu;       // Service rate (1/average service time, packets/s)
    double p;        // Error probability (dimensionless)
    double delta;    // ACK arrival rate (ACKs/s)
    double muack;    // ACK service rate (1/average ACK service time, ACKs/s)
    double Trslen;   // Transient period length (s)
    double Runlen;   // Simulation run length (s)
    int NRUNmin;     // Number of simulation runs

    // Statistics
    Sstat* delay;    // Statistics for average packet delay

    // Internal methods
    void input();                // Read all simulation parameters from user
    void clear_counters();       // Reset counters for each run
    void clear_stats();          // Reset statistics
    void update_stats(double d); // Add a sample to statistics
    void print_trace(int Run);   // Print trace for a single run

    // State variables
    double packets;              // Number of packets processed in current run
    double tot_delay;            // Total delay accumulated in current run
    long total_retransmissions;  // Total number of retransmitted packets (due to errors)
};

#endif 