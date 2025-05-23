/* -*- C++ -*- */
/*******************************************************
		QUEUE.C
*******************************************************/
#include "global.h"      // Definizioni globali
#include <stdio.h>       // Per printf e fprintf
#include "queue.h"       // Definizione della classe queue
#include "rand.h"        // Funzioni per generazione numeri casuali
#include "buffer.h"      // Buffer per la gestione dei pacchetti
#include "event.h"       // Eventi della simulazione (arrival, service)
#include "calendar.h"    // Calendario eventi (priority queue)
#include "easyio.h"      // Funzioni di input/output semplificate


// Variabili globali usate dalla simulazione
calendar* cal;        // Calendario degli eventi (priority queue)
double inter;         // Tempo medio tra due arrivi (inter-arrival time)
double duration;      // Durata media del servizio
double Trslen;        // Durata della fase transitoria (warm-up)
double Runlen;        // Durata di ogni run di simulazione
int NRUNmin;          // Numero minimo di run da eseguire
int NRUNmax;          // Numero massimo di run (non usato esplicitamente)


/* Costruttore della classe queue, inizializza la simulazione */
queue::queue(int argc, char *argv[]): simulator(argc, argv)
{
    cal = new calendar();     // crea il calendario eventi
    buf = new buffer();       // crea il buffer pacchetti (coda)
    delay = new Sstat();      // statistica ritardo (oggetto per medie e varianza)
}

/* Distruttore della classe queue: libera la memoria allocata */
queue::~queue()
{
    delete delay;
    delete cal;
    delete buf;
}

/* Metodo per l’input dei parametri da parte dell’utente */
void queue::input() {
    printf("MODEL PARAMETERS:\n\n");

    // Modello arrivi (qui solo Poisson, valore fisso 1)
    printf("\n Arrivals model:\n");
    printf("1 - Poisson:>\n");
    traffic_model = read_int("", 1, 1, 1);

    // Carico traffico (utilizzo della coda, Erlang)
    load = read_double("Traffic load (Erlang)", 0.4, 0.01, 0.999);

    // Modello servizio (qui solo esponenziale, valore fisso 1)
    printf("\n Service model:\n");
    printf("1 - Exponential:>\n");
    service_model = read_int("", 1, 1, 1);

    // Durata media del servizio in secondi
    duration = read_double("Average service duration (s)", 0.4, 0.01, 100);

    // Calcolo tempo medio tra arrivi in base al carico e durata del servizio
    inter = duration / load;

    printf("SIMULATION PARAMETERS:\n\n");

    // Durata fase transitoria (warm-up) della simulazione
    Trslen = read_double("Simulation transient len (s)", 100, 0.01, 10000);

    // Durata di ogni run (simulazione vera e propria)
    Runlen = read_double("Simulation RUN len (s)", 100, 0.01, 10000);

    // Numero minimo di run da eseguire
    NRUNmin = read_int("Simulation number of RUNs", 5, 2, 100);
}

/* Inizializza la simulazione creando il primo evento di arrivo */
void queue::init()
{
    input(); // Chiede i parametri all'utente

    event* Ev;

    // Primo evento di arrivo con tempo 0 e buffer associato
    Ev = new arrival(0.0, buf);
    cal->put(Ev);   // Inserisce evento nel calendario

    buf->status = 0;  // Segnala che il server è libero (nessun pacchetto in servizio)
}

/* Esecuzione vera e propria della simulazione */
void queue::run() {

    // Variabili globali importate
    extern double Trslen;
    extern double Runlen;
    extern int NRUNmin;
    extern int NRUNmax;

    double clock = 0.0;  // Tempo di simulazione corrente
    event* ev;

    // Fase transitoria: simula fino al tempo Trslen per raggiungere stato stazionario
    while (clock < Trslen) {
        ev = cal->get();   // Estrae evento più prossimo
        ev->body();        // Esegue il corpo dell’evento (gestione arrivo o servizio)
        clock = ev->time;  // Aggiorna l’orologio di simulazione al tempo evento
        delete(ev);        // Libera memoria evento processato
    }

    // Dopo la fase transitoria, si azzerano le statistiche e i contatori
    clear_stats();
    clear_counters();

    int current_run_number = 1;

    // Ciclo dei run di simulazione
    while (current_run_number <= NRUNmin) {
        // Simula fino al tempo corrente + Runlen
        while (clock < (current_run_number * Runlen + Trslen)) {
            ev = cal->get();
            ev->body();
            clock = ev->time;
            delete(ev);
        }
        update_stats();    // Aggiorna le statistiche con i dati del run corrente
        clear_counters();  // Pulisce i contatori per il run successivo
        print_trace(current_run_number); // Stampa i risultati parziali di questo run
        current_run_number++;
    }
}

/* Stampa dei risultati finali della simulazione su file di output */
void queue::results()
{
    extern double Trslen;
    extern double Runlen;
    extern int NRUNmin;
    extern int NRUNmax;

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
    // Stampa media, intervallo di confidenza e valore teorico del ritardo
    fprintf(fpout, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
            delay->mean(),
            delay->confidence(.95),
            delay->confpercerr(.95));
    fprintf(fpout, "D  %2.6f   %2.6f   %.2e %2.6f\n",
            load, delay->mean(), delay->confidence(.95), duration * (load) / (1 - load));
}

/* Stampa traccia del run attuale su file di trace */
void queue::print_trace(int n)
{
    fprintf(fptrc, "*********************************************\n");
    fprintf(fptrc, "                 TRACE RUN %d                \n", n);
    fprintf(fptrc, "*********************************************\n\n");

    fprintf(fptrc, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
            delay->mean(),
            delay->confidence(.95),
            delay->confpercerr(.95));
    fflush(fptrc);  // Forza la scrittura sul file
}

/* Resetta i contatori del buffer */
void queue::clear_counters()
{
    buf->tot_delay = 0.0;  // Somma totale dei ritardi
    buf->tot_packs = 0.0;  // Numero totale pacchetti processati
}

/* Resetta le statistiche di delay */
void queue::clear_stats()
{
    delay->reset();  // Azzeramento dati statistici su delay
}

/* Aggiorna le statistiche sommando il ritardo medio del run corrente */
void queue::update_stats()
{
    *delay += buf->tot_delay / buf->tot_packs;
}

/* Controlla se l'intervallo di confidenza è soddisfatto (criterio di precisione) */
int queue::isconfsatisf(double perc)
{
    return delay->isconfsatisfied(10, .95);  // Esempio di controllo: almeno 10 campioni con confidenza 95%
}
