#include <iostream>
#include <queue>
#include <random>
#include <vector>
#include <iomanip>

using namespace std;

enum EventType { ARRIVAL, SERVICE_END, ACK_ARRIVAL };

struct Event {
    double    time;
    EventType type;
    int       packet_id;
    bool operator>(Event const& o) const { return time > o.time; }
};

struct Packet {
    int   id;
    double arrival_time;
};

struct CompletedPacket {
    double arrival_time;
    double completion_time;
};

class Simulator {
public:
    Simulator(double lambda,
              double mu,
              double p_error,
              bool   stopAndWait,
              double lambda_ack = 0.0,
              double mu_ack     = 0.0)
      : λ(lambda)
      , μ(mu)
      , p_err(p_error)
      , sw(stopAndWait)
      , λ_ack(lambda_ack)
      , μ_ack(mu_ack)
      , gen(rd())
      , expArr(λ)
      , expSrv(μ)
      , expAckArr(λ_ack)
      , expAckSrv(μ_ack)
      , uni01(0.0, 1.0)
      , now(0.0)
      , nextPacketId(0)
      , busy(false)
      , waitingAck(false)
      , maxBuf(0)
      , completed(0)
    {
        // Schedule first data arrival
        schedule(now + expArr(gen), ARRIVAL, nextPacketId++);
    }

    void run(double sim_time) {
        while (!events.empty()) {
            auto e = events.top();
            events.pop();
            if (e.time > sim_time) break;
            now = e.time;
            dispatch(e);
            maxBuf = max<int>(maxBuf, buf.size());
        }

        // Calcolo media delay
        double sum = 0;
        for (auto const& c : finished)
            sum += (c.completion_time - c.arrival_time);
        double avgDelay = finished.empty() ? 0.0 : sum / finished.size();

        // Output
        cout << "\n========== RESULTS ==========\n";
        cout << "Mode: " << (sw ? "stop-and-wait" : "infinite window") << "\n";
        cout << fixed << setprecision(6);
        cout << "Simulated time:        " << sim_time << " s\n";
        cout << "Packets delivered:     " << completed << "\n";
        cout << "Max buffer occupancy:  " << maxBuf << "\n";
        cout << "Average delay (sim):   " << avgDelay << " s\n";
        cout << "==============================\n";
    }

private:
    // Parametri utente
    double λ, μ, p_err;
    bool   sw;      // stop-and-wait?
    double λ_ack;   // tasso Poisson per arrivo ACK
    double μ_ack;   // tasso servizio ACK

    // Stato interno
    double             now;
    int                nextPacketId;
    bool               busy;
    bool               waitingAck;
    int                maxBuf;
    int                completed;

    // Coda di eventi e buffer
    priority_queue<Event, vector<Event>, greater<Event>> events;
    queue<Packet>                                         buf;
    Packet                                                curPkt;
    vector<CompletedPacket>                               finished;

    // RNG
    random_device                      rd;
    mt19937                            gen;
    exponential_distribution<double>   expArr, expSrv, expAckArr, expAckSrv;
    uniform_real_distribution<double>  uni01;

    void schedule(double t, EventType type, int pid) {
        events.push(Event{t, type, pid});
    }

    void dispatch(Event const& e) {
        switch (e.type) {
        case ARRIVAL:     onArrival(e);   break;
        case SERVICE_END: onServiceEnd(e); break;
        case ACK_ARRIVAL: onAck(e);       break;
        }
    }

    void onArrival(Event const& e) {
        // Nuovo pacchetto in ingresso
        Packet p{e.packet_id, now};
        buf.push(p);

        // Schedule next arrival dati
        schedule(now + expArr(gen), ARRIVAL, nextPacketId++);

        // Se server libero e (in infinite-window: sempre) o (in SW: non in attesa ACK)
        if (!busy && (!sw || !waitingAck)) {
            startService();
        }
    }

    void startService() {
        if (buf.empty()) return;
        busy = true;
        waitingAck = false;

        curPkt = buf.front();
        buf.pop();

        // Service time del pacchetto dati
        schedule(now + expSrv(gen), SERVICE_END, curPkt.id);
    }

    void onServiceEnd(Event const& e) {
        busy = false;

        if (!sw) {
            // infinite-window: consegno subito
            completed++;
            finished.push_back({curPkt.arrival_time, now});
            // partiamo con il prossimo pacchetto, se c'è
            startService();
        } else {
            // stop-and-wait: ora attendiamo ACK
            waitingAck = true;
            double t_ack = now + expAckArr(gen);   // arrivo ACK
            schedule(t_ack, ACK_ARRIVAL, curPkt.id);
        }
    }

    void onAck(Event const& e) {
        if (!sw || e.packet_id != curPkt.id) return;

        double u = uni01(gen);
        if (u < p_err) {
            // ACK perso/errato → ritrasmissione del dato
            double t_retx = now + expSrv(gen);
            schedule(t_retx, SERVICE_END, curPkt.id);
        } else {
            // ACK valido → consegno il pacchetto
            completed++;
            finished.push_back({curPkt.arrival_time, now});
            waitingAck = false;
            // posso inviare il prossimo pacchetto (se c'è)
            startService();
        }
    }
};


int main() {
    cout << fixed << setprecision(4);
    double λ, μ, p;
    cout << "Enter data arrival rate λ        : "; cin  >> λ;
    cout << "Enter data service rate μ        : "; cin  >> μ;
    cout << "Enter packet error probability p : "; cin  >> p;

    cout << "\nSelect ARQ mode:\n"
         << "  1) Infinite-window (no ACK blocking)\n"
         << "  2) Stop-and-wait\n"
         << "Choice [1/2]: ";
    int choice; cin >> choice;

    bool sw = (choice == 2);
    double λ_ack = 0.0, μ_ack = 0.0;
    if (sw) {
        cout << "Enter ACK arrival rate λ_ACK   : "; cin >> λ_ack;
        cout << "Enter ACK service rate μ_ACK   : "; cin >> μ_ack;
    }

    double sim_time;
    cout << "\nEnter total simulation time (s): "; cin >> sim_time;

    Simulator sim(λ, μ, p, sw, λ_ack, μ_ack);
    sim.run(sim_time);

    return 0;
}
