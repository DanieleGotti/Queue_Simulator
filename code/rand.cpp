/* -*- C++ -*- */
#include <math.h>    // Funzioni matematiche come ceil, exp, pow
#include "rand.h"    // Funzioni per numeri casuali (PSEUDO)

/*
 * Array di semi per il generatore di numeri casuali
 * Semina iniziale per differenti flussi di casualità
 */
int lseed[11] = {
    0,
    428956419,
    1954324947,
    1145661099,
    1835732737,
    794161987,
    1329531353,
    200496737,
    633816299,
    1410143363,
    1282538739
};

/*
 * Funzione arrotonda:
 * arrotonda il numero reale x al più vicino intero usando la regola
 * standard: se la parte decimale è >= 0.5 arrotonda per eccesso, altrimenti per difetto
 */
int arrotonda(double x)
{
    double up;

    up = ceil(x); // parte intera superiore (primo intero >= x)

    if ((up - x) < 0.5)
        return (int)up;       // arrotonda per eccesso
    else
        return (int)(up - 1); // arrotonda per difetto
}

/*
 * Funzione transmit:
 * simula una trasmissione con probabilità di successo g
 * genera un numero casuale (prob) tra 0 e 1 e restituisce 1 se prob<g (successo),
 * altrimenti 0 (fallimento)
 */
int transmit(double g)
{
    double prob;
    PSEUDO(SEED, prob);  // genera numero casuale uniforme [0,1)

    if (prob < g)
        return 1;  // successo trasmissione
    else
        return 0;  // fallimento trasmissione
}

/*
 * Funzione fact (fattoriale):
 * calcola ricorsivamente il fattoriale di un intero i
 * fact(0) = 1, fact(1) = 1, fact(i) = i * fact(i-1)
 */
int fact(int i)
{
    if (i <= 1)
        return 1;
    else
        return i * fact(i - 1);
}

/*
 * Funzione n_interf_by_poisson:
 * calcola il numero di interferenze (eventi) secondo una distribuzione di Poisson con media 'average'
 * usa il metodo di inversione della CDF discreta di Poisson:
 * - genera un numero casuale y uniformemente distribuito (0,1)
 * - cerca x tale che y <= P(X=x), dove P(X=x) è la probabilità di Poisson
 */
int n_interf_by_poisson(double average)
{
    double y, out;
    int x;

    PSEUDO(SEED, y);  // genera un numero casuale uniforme [0,1)

    for (x = 1;; x++) {
        // calcola la probabilità di x eventi per Poisson: exp(-average) * average^x / x!
        out = exp(-average) * pow(average, (double)x) / fact(x);

        if (y <= out)
            break;  // trovato il valore x corrispondente a y
    }

    return x;
}
