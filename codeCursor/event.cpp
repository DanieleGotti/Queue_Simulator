/***********************************************************************
		EVENT.C
***********************************************************************/

#include "event.h"
#include "buffer.h"
#include "calendar.h"
#include "rand.h"

// Variabili globali dichiarate altrove
extern calendar* cal;       // Puntatore al calendario degli eventi
extern double inter;        // Intervallo medio di arrivo dei pacchetti
extern double duration;     // Durata media del servizio

// Metodo body() della classe arrival: gestisce l'arrivo di un pacchetto
void arrival::body(){
    event* ev;

    // Generazione del prossimo arrivo esponenziale (tempo inter-arrivo)
    double esito;
    GEN_EXP(SEED, inter, esito);  // Calcola esito = tempo per prossimo arrivo
    ev = new arrival(time + esito, buf); // Crea un nuovo evento di arrivo
    cal->put(ev); // Inserisce il nuovo arrivo nel calendario degli eventi

    // Creazione di un nuovo pacchetto
    packet* pack = new packet(time);

    // Se il buffer è pieno o è occupato da un altro pacchetto in servizio
    if (buf->full() || buf->status) {
        buf->insert(pack);  // Inserisce il pacchetto in coda
    } else {
        // Il buffer è libero, si avvia direttamente il servizio del pacchetto
        buf->tot_packs += 1.0;  // Incrementa il contatore dei pacchetti serviti
        delete pack; // Il pacchetto viene servito subito, non va in coda

        // Genera tempo di servizio esponenziale
        GEN_EXP(SEED, duration, esito);
        ev = new service(time + esito, buf); // Crea evento di servizio
        cal->put(ev);  // Inserisce l'evento di servizio nel calendario

        buf->status = 1; // Imposta lo stato del buffer a "occupato"
    }
}

// Metodo body() della classe service: gestisce il completamento del servizio
void service::body(){
    packet* pack;
    pack = buf->get(); // Estrae il prossimo pacchetto dalla coda

    event* ev;
    double esito;

    // Se è presente un pacchetto nella coda
    if (pack != NULL) {
        // Genera tempo di servizio esponenziale per il pacchetto successivo
        GEN_EXP(SEED, duration, esito);
        ev = new service(time + esito, buf); // Crea nuovo evento di servizio
        cal->put(ev);  // Inserisce l'evento nel calendario

        // Aggiorna la statistica del ritardo totale accumulato
        buf->tot_delay += time - pack->get_time();

        // Incrementa il numero totale di pacchetti serviti
        buf->tot_packs += 1.0;

        // Elimina il pacchetto servito
        delete pack;
    } else {
        // Nessun pacchetto in attesa, il buffer diventa libero
        buf->status = 0;
    }
}
