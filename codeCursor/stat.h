/*****************************************************************************

                S T A T . H

    Contiene la definizione della classe:
          - Sstat: modificata ed estesa dalla classe SampleStatistic
            della libreria GNU.

*****************************************************************************/
#ifndef _STAT_H
#define _STAT_H

#include <math.h>

//----------------------------------------------------------------------------
//              CLASSE Sstat
//----------------------------------------------------------------------------

class Sstat {
protected:
    int n;          // numero di campioni
    double x;       // somma dei campioni
    double x2;      // somma dei quadrati dei campioni
    double last;    // ultimo campione inserito
    double minValue;// valore minimo osservato
    double maxValue;// valore massimo osservato

public:
    Sstat();
    virtual ~Sstat();

    virtual void reset();               // azzera tutte le statistiche
    virtual void operator+=(double);   // aggiunge un nuovo campione

    int num_samples();          // restituisce il numero di campioni
    double last_sample();       // restituisce l'ultimo campione
    double sum();               // somma di tutti i campioni
    double mean();              // media
    double stddev();            // deviazione standard
    double var();               // varianza
    double min();               // valore minimo
    double max();               // valore massimo

    // Calcolo degli intervalli di confidenza (input: percentuale o valore)
    double confidence(int p_percentage);
    double confidence(double p_value);
    double confpercerr(int p_percentage);
    double confpercerr(double p_value);

    // Verifica se l'errore percentuale è sotto una certa soglia
    int isconfsatisfied(double perc=1.0, double pconf=.95);
};

inline Sstat::Sstat() { reset(); }
inline Sstat::~Sstat() {}

inline int Sstat::num_samples() { return n; }
inline double Sstat::last_sample() { return last; }
inline double Sstat::min() { return minValue; }
inline double Sstat::max() { return maxValue; }
inline double Sstat::sum() { return x; }

inline double Sstat::confidence(int p_percentage) {
    return confidence(double(p_percentage) * 0.01);
}

inline double Sstat::confpercerr(int p_percentage) {
    return confpercerr(double(p_percentage) * 0.01);
}

inline int Sstat::isconfsatisfied(double perc, double pconf) {
    return (confpercerr(pconf) < perc);
}

#endif  /* _STAT_H */
