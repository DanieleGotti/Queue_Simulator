/*----------------------------------------------------------------------*/
/*                                                                      */
/*                               easyio.h                               */
/*                                                                      */
/*      Header per il pacchetto easyio C++ che fornisce funzioni        */
/*      di input/output semplificato per vari tipi di dato               */
/*                                                                      */
/*----------------------------------------------------------------------*/

#ifndef _EASYIOPLUS_H
#define _EASYIOPLUS_H

// Costanti per lunghezze massime di input accettati
#define EASYIO_MAX_INT_LEN       9    /* lunghezza massima per interi (fino a 10^10-1) */
#define EASYIO_MAX_FLOAT_LEN     15   /* lunghezza massima per numeri in virgola mobile */
#define EASYIO_MAX_STR_LEN       80   /* lunghezza massima per stringhe */
#define EASYIO_MAX_BUF_LEN       80   /* lunghezza massima per buffer generici */

// Limiti per valori interi gestiti
#define EIO_MIN_INT              -1000000000
#define EIO_MAX_INT              1000000000

// Codici di ritorno per funzioni
#define EASYIO_OKAY              0    // esecuzione andata a buon fine
#define EASYIO_ERROR             1    // errore
#define EASYIO_DEFAULT           2    // valore di default

// Definizione booleana semplice
#define true                    1
#define false                   0

// Dichiarazioni delle funzioni esportate dal pacchetto easyio

// Funzioni di scansione di input da tastiera e parsing
extern int scandigit(int *);
extern int scanint(int *);
extern int scanlong(long *);
extern int scandouble(double *);
extern int scanstring(char *);

// Funzioni di utilità per output e controllo
extern void clear_screen();   // Pulisce lo schermo
extern void beep();           // Suono di beep
extern void pausa();          // Pausa, aspetta input
extern char readln();         // Legge una riga

// Funzioni di lettura di booleani, numeri, stringhe con diversi parametri
extern short read_bool(char *, short);
extern short read_bool(char *);

extern int read_digit(char *, int, int, int);
extern int read_digit(char *, int);
extern int read_digit(char *);

extern int read_int(char *, char *, int, int, int);
extern int read_int(char *, int, int, int);
extern int read_int(char *, char *, int);
extern int read_int(char *, int);
extern int read_int(char *);

extern long read_long(char *, char *, long, long, long);
extern long read_long(char *, long, long, long);
extern long read_long(char *, char *, long);
extern long read_long(char *, long);
extern long read_long(char *);

extern double read_double(char *, char *, double, double, double);
extern double read_double(char *, double, double, double);
extern double read_double(char *, char *, double);
extern double read_double(char *, double);
extern double read_double(char *);

extern char * read_string(char *, char *);
extern char * read_string(char *);

#endif    // _EASYIOPLUS_H
