/* -*- C++ -*- */
/*******************************************************
		SIMULATOR.C
	Abstract class for simulation program
*******************************************************/

#include <stdio.h>   // Input/output standard
#include "simulator.h"
#include <stdlib.h>  // Funzioni standard di utilità come exit()

/*
 * Funzione inline GoFatal:
 * Stampa un messaggio di uso corretto del programma
 * e termina l'esecuzione con codice di errore -1.
 * Viene chiamata quando gli argomenti in input non sono corretti.
 */
inline void GoFatal() {
    fprintf(stderr, "usage: -o output_file -t trace_file\n");
    exit(-1);
}

/*
 * Funzione inline read_opt:
 * Controlla che una stringa passata come opzione sia del formato corretto:
 * deve iniziare con '-' e contenere un singolo carattere opzione,
 * altrimenti chiama GoFatal per terminare il programma.
 * Restituisce il carattere opzione.
 */
inline char read_opt(char* s) {
    if (s[0] != '-')  GoFatal();   // deve iniziare con '-'
    if (s[2] != '\0') GoFatal();   // deve avere solo un carattere opzione dopo '-'
    return s[1];                   // ritorna il carattere opzione
}

/*
 * Metodo simulator::read_args:
 * Legge e interpreta gli argomenti della linea di comando passati a main.
 * Si aspetta opzioni di tipo '-o output_file' e '-t trace_file'.
 * Se non trovate, imposta i file di output di default.
 * In caso di errori o opzioni duplicate, chiama GoFatal.
 */
void simulator::read_args(int argc,char* argv[])
{
    int f_o = -1;  // flag per opzione output file
    int f_t = -1;  // flag per opzione trace file
    char c;
    int base = 1;  // parte da argv[1] (skip argv[0] che è il nome programma)
    argc--;        // numero argomenti rimanenti da leggere

    while (argc > 0) {
        c = read_opt(argv[base]); // legge l'opzione '-o' o '-t'
        argc--;
        base++;

        switch (c) {
            case 'o': // opzione output file
                if (f_o != -1) GoFatal();       // errore se già settata
                if (argc == 0) GoFatal();       // errore se mancano parametri
                fpout = fopen(argv[base], "w"); // apre file in scrittura
                argc--;
                base++;
                f_o = 1;
                break;

            case 't': // opzione trace file
                if (f_t != -1) GoFatal();       // errore se già settata
                if (argc == 0) GoFatal();       // errore se mancano parametri
                fptrc = fopen(argv[base], "w"); // apre file in scrittura
                argc--;
                base++;
                f_t = 1;
                break;

            default:  // opzione non riconosciuta
                GoFatal();
        }
    }

    // Se non è stato specificato file output, usa stdout
    if (f_o == -1) fpout = stdout;

    // Se non è stato specificato file trace, usa lo stesso file di output
    if (f_t == -1) fptrc = fpout;
}

/*
 * Costruttore della classe simulator:
 * Inizializza i puntatori ai file a NULL,
 * poi chiama read_args per processare gli argomenti passati al programma.
 */
simulator::simulator(int argc,char* argv[])
{
    fptrc = NULL;  // puntatore file per trace inizialmente NULL
    fpout = NULL;  // puntatore file per output inizialmente NULL
    read_args(argc, argv);  // legge e apre file da linea di comando
}
