#include <iostream>
#include <queue>
#include <random>
#include <functional>
#include <vector>
#include <iomanip>

using namespace std;

enum EventType { ARRIVAL, SERVICE_END, ACK_ARRIVAL };

struct Event {
    double time;
    EventType type;
    int packet_id; // id pacchetto associato, -1 per eventi generici

    bool operator>(const Event& e) const { return time > e.time; }
};

struct Packet {
    int id;
    double arrival_time;  // tempo arrivo
    double service_start_time;
};

class Simulator {
public:
    Simulator(double lambda, double mu, double delta, double mu_ack, double p_error, double sim_time)
        : lambda(lambda), mu(mu), delta(delta), mu_ack(mu_ack), p_error(p_error), sim_time(sim_time),
          gen(rd()), expArrival(lambda), expService(mu), expAck(delta), expAckService(mu_ack), uniform(0.0,1.0) {
        currentTime = 0.0;
        nextPacketId = 0;
        busy = false;
        waitingAck = false;
        maxBufferSize = 0;
        processedPackets = 0;
    }

    void run() {
        // Schedule first arrival
        scheduleEvent(currentTime + expArrival(gen), ARRIVAL, nextPacketId);

        while (!eventQueue.empty() && currentTime < sim_time) {
            Event e = eventQueue.top();
            eventQueue.pop();
            currentTime = e.time;

            if (currentTime > sim_time) break;

            processEvent(e);
            if ((int)buffer.size() > maxBufferSize) maxBufferSize = buffer.size();
        }

        double sumTimes = 0.0;
        for (auto& pkt : completedPackets) {
            sumTimes += (pkt.ack_time - pkt.arrival_time);
        }
        double avgDelaySim = (completedPackets.size() > 0) ? (sumTimes / completedPackets.size()) : 0.0;

        double rho = lambda * (1.0/mu + 1.0/delta);
        double theoreticalDelay = (1.0/mu + (p_error/(1-p_error))*(1.0/mu) + 1.0/delta) / (1 - rho);

        // Stampa risultati
        cout << "*********************************************\n";
        cout << "           SIMULATION RESULTS\n";
        cout << "*********************************************\n";
        cout << fixed << setprecision(6);
        cout << "Total time simulated:        " << sim_time << " s\n";
        cout << "Packets transmitted:         " << processedPackets << "\n";
        cout << "Average delay (simulated):   " << avgDelaySim << " s\n";
        cout << "Average delay (theoretical): " << theoreticalDelay << " s\n";
        cout << "Max buffer size reached:     " << maxBufferSize << "\n";
        cout << "*********************************************\n";
    }

private:
    struct CompletedPacket {
        double arrival_time;
        double ack_time;
    };

    double lambda, mu, delta, mu_ack, p_error, sim_time;
    double currentTime;
    int nextPacketId;
    int processedPackets;

    bool busy;
    bool waitingAck;

    int maxBufferSize;

    priority_queue<Event, vector<Event>, greater<Event>> eventQueue;
    queue<Packet> buffer;
    Packet currentPacket;

    random_device rd;
    mt19937 gen;
    exponential_distribution<> expArrival;
    exponential_distribution<> expService;
    exponential_distribution<> expAck;        // per arrivo ACK (Poisson)
    exponential_distribution<> expAckService; // durata servizio ACK
    uniform_real_distribution<> uniform;

    vector<CompletedPacket> completedPackets;

    void scheduleEvent(double time, EventType type, int packet_id) {
        eventQueue.push(Event{time, type, packet_id});
    }

    void processEvent(const Event& e) {
        switch (e.type) {
            case ARRIVAL:
                handleArrival(e);
                break;
            case SERVICE_END:
                handleServiceEnd(e);
                break;
            case ACK_ARRIVAL:
                handleAckArrival(e);
                break;
        }
    }

    void handleArrival(const Event& e) {
        Packet pkt;
        pkt.id = e.packet_id;
        pkt.arrival_time = currentTime;
        buffer.push(pkt);

        // Schedule next arrival
        nextPacketId++;
        double nextArr = currentTime + expArrival(gen);
        if (nextArr < sim_time) {
            scheduleEvent(nextArr, ARRIVAL, nextPacketId);
        }

        if (!busy && !waitingAck) {
            startService();
        }
    }

    void startService() {
        if (buffer.empty()) return;
        busy = true;
        waitingAck = false;
        currentPacket = buffer.front();
        buffer.pop();
        currentPacket.service_start_time = currentTime;

        double serviceTime = expService(gen);
        scheduleEvent(currentTime + serviceTime, SERVICE_END, currentPacket.id);
    }

    void handleServiceEnd(const Event& e) {
        busy = false;
        waitingAck = true;

        // Simuliamo durata servizio ACK (si somma al tempo ACK)
        double ackArrivalTime = currentTime + expAck(gen) + expAckService(gen);
        scheduleEvent(ackArrivalTime, ACK_ARRIVAL, currentPacket.id);
    }

    void handleAckArrival(const Event& e) {
        if (e.packet_id != currentPacket.id) return;

        double rnd = uniform(gen);
        if (rnd < p_error) {
            // Errore: ritrasmetto dopo un altro ACK
            double nextAckTime = currentTime + expAck(gen) + expAckService(gen);
            scheduleEvent(nextAckTime, ACK_ARRIVAL, currentPacket.id);
            return;
        }

        // Pacchetto correttamente ricevuto
        waitingAck = false;
        processedPackets++;
        completedPackets.push_back({currentPacket.arrival_time, currentTime});

        if (!buffer.empty()) {
            startService();
        }
    }
};

int main() {
    // Parametri
    double lambda = 0.2;       // Tasso di arrivo pacchetti
    double mu = 0.3;              // Tasso di servizio (trasmissione)
    double delta = 5;           // Tasso arrivo ACK (Poisson)
    double mu_ack = 5;          // Tasso servizio ACK (durata media servizio ACK)
    double p_error = 0.1;      // Probabilità errore trasmissione
    double sim_time = 10000.0;  // Durata simulazione in secondi

    Simulator sim(lambda, mu, delta, mu_ack, p_error, sim_time);
    sim.run();

    return 0;
}
