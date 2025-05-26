/*****************************************************************************

			       S T A T . C

	Modified from class SampleStatistic of GNU library

*****************************************************************************/

#include "stat.h"

#ifndef HUGE_VAL
#define HUGE_VAL 1.0e+99   // Definisce un valore molto grande, usato per errori o infiniti
#endif

/*
 * Funzione tval:
 * Calcola il valore critico t della distribuzione t di Student
 * data una probabilità p e i gradi di libertà df.
 * Basata su Peizer & Pratt JASA, vol63, p1416.
 * Utilizzata per calcolare intervalli di confidenza.
 */
double tval(double p, int df) {
  	double t;
  	int positive = p >= 0.5;      // memorizza se p è nella metà superiore
  	p = (positive)? 1.0 - p : p; // lavora sempre con p <= 0.5 (simmetria t-distribuzione)

  	if (p <= 0.0 || df <= 0)
		t = HUGE_VAL;               // valore invalido per p o df, ritorna "infinito"
  	else if (p == 0.5)
		t = 0.0;                    // mediana della distribuzione t
  	else if (df == 1)
		t = 1.0 / tan((p + p) * 1.57079633); // caso speciale df=1 (Cauchy distribution)
  	else if (df == 2)
		t = sqrt(1.0 / ((p + p) * (1.0 - p)) - 2.0); // caso speciale df=2
  	else {
    		double ddf = df;
    		double a = sqrt(log(1.0 / (p * p)));
    		double aa = a * a;

		// Approssimazione di Peizer & Pratt per df > 2
    		a = a - ((2.515517 + 0.802853*a + 0.010328*aa) /
             		(1.0 + 1.432788*a + 0.189269*aa + 0.001308*aa*a));
    		t = ddf - 0.666666667 + 1.0 / (10.0 * ddf);
    		t = sqrt(ddf * (exp(a*a * (ddf - 0.833333333)/(t * t)) - 1.0));
  	}
  	return (positive)? t : -t;  // restituisce t con il segno corretto
}

/*
 * Metodo Sstat::reset:
 * Reimposta lo stato della statistica campionaria:
 * conta, somma, somma dei quadrati, min e max valori.
 */
void Sstat::reset() {
    	n = 0;
		x = x2 = last = 0.0;
    	maxValue = -HUGE_VAL;   // minore valore possibile iniziale per max
    	minValue = HUGE_VAL;    // maggiore valore possibile iniziale per min
}

/*
 * Operatore += per aggiungere un nuovo valore alle statistiche.
 * Aggiorna:
 * - numero di valori n
 * - somma x
 * - somma dei quadrati x2
 * - valore ultimo last
 * - minimo e massimo finora
 */
void Sstat::operator+=(double value) {
    	n += 1;
   	last = value;
    	x += value;
    	x2 += (value * value);
    	if (minValue > value) minValue = value;
    	if (maxValue < value) maxValue = value;
}

/*
 * Calcola e restituisce la media dei valori raccolti.
 * Se nessun dato, restituisce 0.
 */
double Sstat::mean() {
    	if (n > 0) return (x / n);
    	else return 0.0;
}

/*
 * Calcola la varianza campionaria.
 * Se meno di 2 dati, restituisce 0.
 */
double Sstat::var() {
    	if (n > 1) return ((x2 - ((x * x) / n)) / (n - 1));
    	else return 0.0;
}

/*
 * Calcola la deviazione standard come radice quadrata della varianza.
 */
double Sstat::stddev() {
   	return sqrt(var());
}

/*
 * Calcola l’intervallo di confidenza per la media al livello p_value.
 * Usa la funzione tval per ottenere il valore critico t.
 * Se non possibile calcolare, restituisce HUGE_VAL.
 */
double Sstat::confidence(double p_value) {
  	int df = n - 1;                // gradi di libertà
  	if (df <= 0) return HUGE_VAL; // non si può calcolare
  	double t = tval((1.0 + p_value) * 0.5, df);
  	if (t == HUGE_VAL) return t;
  	else return (t * stddev()) / sqrt(double(n));
}

/*
 * Calcola l’errore percentuale relativo all’intervallo di confidenza
 * rispetto alla media.
 * Se la media è 0, restituisce 100% (non calcolabile).
 */
double Sstat::confpercerr(double p_value) {
   	double Mean = mean();
   	if (Mean == 0.0) return 100.0;		// impossibile calcolare % errore
   	else return ((confidence(p_value) * 100.0) / Mean);
}
