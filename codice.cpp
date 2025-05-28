Ho questo codice di un simulatore GG1 coda infinita funzionante che per ogni track calcola Average Delay e intervalli di confidenza. 
Ora voglio che aggiugni una nuova classe per implementare Stop and wait sistema GG1 che per ogni track calcola Average Deley e intervalli di confidenza
dando come input, oltre a tutti i dati del sistema non stop and wait, anche lambdaACK, muACK e probabilità errore, tutti gli altri input sono in comune.
Voglio che ci sia in input un selettore: se 1 viene eseguito il primo simulatore senza alcuna tua modifica, se 2 parte lo stop and wait che hai creato tu.
Inoltre ricorda di memorizzare le funzioni del primo sistema: ad esempio se c'è già un generatore di arrivi nel codice che ti ho dato chiama quella fuznione,
non crearne un altra, e cosi per tutto il resto.



/***************************************************************************
BUFFER.CPP
***************************************************************************/

// Inclusione del file di intestazione associato alla classe buffer
#include "buffer.h"

// Costruttore della classe buffer
buffer::buffer(){
	head = NULL;         // Puntatore alla testa della coda inizializzato a NULL
	last = NULL;         // Puntatore alla coda (ultimo elemento) inizializzato a NULL
	status = 0;          // Stato del buffer, inizializzato a 0 (ad esempio libero o vuoto)
	tot_delay = 0.0;     // Ritardo totale accumulato, inizializzato a 0
	tot_packs = 0.0;     // Numero totale di pacchetti gestiti, inizializzato a 0
}

// Metodo per inserire un pacchetto nel buffer
void buffer::insert(packet* pack){
	if (head == NULL) {        // Se il buffer è vuoto
		head = pack;          // Il nuovo pacchetto diventa la testa
		last = pack;          // e anche l'ultimo
		last->next = head;    // Si crea un collegamento circolare (solo un elemento)
	}
	else {
		last->next = pack;    // Il pacchetto corrente diventa il successore dell'ultimo
		last = pack;          // Il pacchetto appena inserito diventa il nuovo ultimo
		last->next = head;    // Si ristabilisce il collegamento circolare
	}
}

// Metodo per rimuovere e restituire un pacchetto dalla testa del buffer
packet* buffer::get(){
	packet* pack;

	if (head == NULL)          // Se il buffer è vuoto
		return NULL;           // Non c'è nulla da restituire

	if (last == head) {        // Se c'è un solo pacchetto nel buffer
		pack = head;           // Lo si salva per il ritorno
		last = NULL;           // Il buffer torna vuoto
		head = NULL;
	}
	else {
		pack = head;           // Salva il pacchetto da rimuovere
		head = head->next;     // Avanza la testa al prossimo pacchetto
		last->next = head;     // Ristabilisce il collegamento circolare
	}

	return pack;               // Restituisce il pacchetto rimosso
}

;

/***************************************************************************
BUFFER.H
***************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#include "packet.h"  // Include la definizione della classe packet

// Classe buffer:
// Gestisce una lista di pacchetti (packet) in FIFO tramite due puntatori:
// head -> primo pacchetto della lista
// last -> ultimo pacchetto inserito nella lista
class buffer {

	packet* head;  // Puntatore al primo pacchetto nella coda
	packet* last;  // Puntatore all'ultimo pacchetto nella coda

public:
	int status;   // Stato generico del buffer, utilizzabile esternamente

public:
	buffer();    // Costruttore: inizializza il buffer
	~buffer(){}  // Distruttore vuoto (da implementare se necessario)

	// Inserisce un pacchetto alla fine della coda
	void insert(packet* pack);

	// Restituisce (e rimuove) il primo pacchetto dalla coda
	packet* get();

	// Restituisce il puntatore al primo pacchetto (head) senza rimuoverlo
	packet* full() { return head; }

	// Statistiche del buffer
	double tot_delay;   // Totale ritardo accumulato dei pacchetti
	double tot_packs;   // Numero totale di pacchetti processati
};

#endif

;

/* -*- C++ -*- */
/*******************************************************
		CALENDAR Cpp
*******************************************************/

// Inclusione dei file di intestazione globali e della classe calendar
#include "global.h"
#include "calendar.h"

// Metodo per ottenere il prossimo evento (rimozione dalla testa della lista)
event* calendar::get() {
	if (head == NULL)               // Se il calendario è vuoto
		return NULL;               // Non c'è evento da restituire

	event* ev;

	if (head == last) {            // Se c'è un solo evento nella lista
		ev = head;                // Salva l'evento da restituire
		head = NULL;              // Ripristina lista vuota
		last = NULL;
		return ev;                // Restituisce l'evento
	}

	ev = head;                     // Salva l'evento corrente
	head = head->next;            // Avanza la testa
	last->next = head;            // Mantiene la struttura circolare
	return ev;                    // Restituisce l'evento
}

// Metodo per inserire un nuovo evento mantenendo l'ordine temporale
void calendar::put(event* New) {
	event* temp = head;           // Puntatore temporaneo per scorrere la lista
	event* pippo;                 // Puntatore ausiliario (inutile qui, ma inizializzato)
	pippo = New;

	if (head == NULL) {           // Inserimento nella lista vuota
		head = New;
		head->next = New;        // Lista circolare con un solo elemento
		last = New;
	}
	else if (New->time < head->time) {  // Inserimento in testa
		New->next = head;
		head = New;
		last->next = head;       // Aggiorna la circolarità
	}
	else if (last == head) {      // Se c'è un solo evento nella lista
		if (New->time < head->time) {
			head = New;
			head->next = last;
			last->next = head;
		}
		else {
			last = New;
			head->next = last;
			last->next = head;
		}
	}
	else if (last->time < New->time) {  // Inserimento in fondo
		last->next = New;
		last = New;
		last->next = head;         // Chiude la circolarità
	}
	else {                         // Inserimento in posizione intermedia
		while (temp->next->time < New->time)
			temp = temp->next;    // Cerca la posizione corretta
		New->next = temp->next;   // Inserisce l'evento
		temp->next = New;
	}
}

;

/*******************************************************
CALENDAR H
*******************************************************/

#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "simulator.h"
#include "event.h"

// Classe calendar:
// Gestisce una lista concatenata di eventi (event) in ordine temporale,
// utilizzata per mantenere il calendario degli eventi nella simulazione.
class calendar {

	event* head;  // Puntatore al primo evento nella lista (evento con tempo minore)
	event* last;  // Puntatore all'ultimo evento nella lista (evento con tempo maggiore)

public:

	calendar();   // Costruttore: inizializza lista vuota
	~calendar();  // Distruttore: libera la memoria degli eventi rimasti

	// Estrae (rimuove) e restituisce il primo evento della lista (evento più prossimo nel tempo)
	event* get();

	// Inserisce un nuovo evento ordinandolo nella lista in base al tempo
	void put(event* New_event);
};


// Costruttore inline: inizializza la lista come vuota
inline calendar::calendar() {
	head = NULL;
	last = NULL;
}

// Distruttore inline: elimina tutti gli eventi ancora presenti nella lista
inline calendar::~calendar() {
	event* temp = head;
	// Disconnette il puntatore next dell'ultimo evento per evitare accessi invalidi
	last->next = NULL;

	// Ciclo di cancellazione di tutti gli eventi nella lista
	while (temp != NULL) {
		temp = temp->next;  // Passa all'evento successivo
		delete head;        // Cancella l'evento corrente
		head = temp;        // Aggiorna head al prossimo evento
	}
}

#endif

;

/************************************************************************
	THIS FILE COLLAPSES THREE MODULES, FORMERLY CALLED
	EASYIO_PRIM.Cpp, EASYIO_SCAN.Cpp and EASYIO_READ.Cpp
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "easyio.h"

/*----------------------------------------------------------------------*/
/* easyio_prim.Cpp                                                        */
/* Funzioni di I/O basilari per facilitare l'interazione con l'utente. */
/*----------------------------------------------------------------------*/

// Pulisce lo schermo usando il comando "clear"
void clear_screen() {
        system("clear");
	return;
	}

// Legge un singolo carattere dall'input e scarta tutto il resto fino a <CR>
char readln() {
	char d = getchar();    // Legge il primo carattere
	char c = d;
	while (d!='\n')        // Scarta i successivi fino a newline
		d=getchar();
	return (c);            // Restituisce il primo carattere letto
	}

// Emette un segnale acustico (beep)
void beep() {
	putchar(7); // ASCII Bell
	return;
	}

// Attende che l'utente prema <RETURN> prima di continuare
void pausa() {
	fprintf(stdout,"\n\n");
        fprintf(stdout,"     *****   Press <RETURN> to continue   *****");
	readln();         // Attende input
	clear_screen();   // Pulisce schermo
	return;
	}

/*----------------------------------------------------------------------*/
/* easyio_scan.CPP                                                        */
/* Funzioni di input utente con validazione simili a scanf().          */
/*----------------------------------------------------------------------*/

// Legge una cifra (0-9) e la converte in intero
int	scandigit (int *parm) {
	int	c = readln();  // Legge un carattere
	switch (c)  {
		case '\n' :
			return(EASYIO_DEFAULT);  // Nessun input
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			*parm = c - '0';        // Converte da char a int
			return(EASYIO_OKAY);
		default:
			return(EASYIO_ERROR);   // Input non valido
		}
	}

// Legge un numero intero da input, con validazione
int	scanint (int *parm){
	int 	c;
	int	buf[EASYIO_MAX_BUF_LEN];
	int 	buf_len;

	// Riempie buffer con caratteri fino a newline o commento
	for (buf_len=0; ((c=getchar()) != '\n') && (c != '#') &&
		(buf_len < EASYIO_MAX_BUF_LEN); buf_len++) buf[buf_len]=c;
	if (c=='#') c=readln();	// Scarta commento

	// Gestione casi speciali
	switch (buf_len)  {
		case 0: return(EASYIO_DEFAULT);
		case EASYIO_MAX_BUF_LEN: return(EASYIO_ERROR);
		default: break;
		}

	int 	intero=0, count=0;
	short 	neg=1; // Segno

	// Converte caratteri in numero intero
	for (int i=0; i<buf_len; i++) {
		c=buf[i];
		if ((c!=' ') && (c!='\t')) {
			if (count==-1) return(EASYIO_ERROR);
			else  count++;
			}
		if (count > EASYIO_MAX_INT_LEN) return(EASYIO_ERROR);

		if (c=='-') neg=-1;
		else if ((c >= '0') && (c <= '9'))
			intero=(intero*10)+buf[i]-'0';
		else if ((c==' ') || (c=='\t')) {
			if (count!=0) count=-1;
			}
		else return(EASYIO_ERROR);
		}

	*parm=neg*intero;
	return(EASYIO_OKAY);
	}

// Versione di scanint per numeri long
int	scanlong (long *parm){
	int 	c;
	int	buf[EASYIO_MAX_BUF_LEN];
	int 	buf_len;

	for (buf_len=0; ((c=getchar()) != '\n') && (c != '#') &&
		(buf_len < EASYIO_MAX_BUF_LEN); buf_len++) buf[buf_len]=c;
	if (c=='#') c=readln();

	switch (buf_len)  {
		case 0: return(EASYIO_DEFAULT);
		case EASYIO_MAX_BUF_LEN: return(EASYIO_ERROR);
		default: break;
		}

	long 	intero=0;
	int 	count=0;
	short 	neg=1;

	for (int i=0; i<buf_len; i++) {
		c=buf[i];
		if ((c!=' ') && (c!='\t')) {
			if (count==-1) return(EASYIO_ERROR);
			else count++;
			}
		if (count > EASYIO_MAX_INT_LEN) return(EASYIO_ERROR);

		if (c=='-') neg=-1;
		else if ((c >= '0') && (c <= '9'))
			intero=(intero*10)+buf[i]-'0';
		else if ((c==' ') || (c=='\t')) {
			if (count!=0) count=-1;
			}
		else return(EASYIO_ERROR);
		}

	*parm=neg*intero;
	return(EASYIO_OKAY);
	}

// Legge un numero in virgola mobile (double) con validazione
int	scandouble (double *parm) {
	int 	c, buf_len;
	int	buf[EASYIO_MAX_BUF_LEN];

	for (buf_len=0; ((c=getchar()) != '\n') && (c != '#') &&
		(buf_len < EASYIO_MAX_BUF_LEN); buf_len++) buf[buf_len]=c;
	if (c=='#') c=readln();

	switch (buf_len)  {
		case 0: return(EASYIO_DEFAULT);
		case EASYIO_MAX_BUF_LEN: return(EASYIO_ERROR);
		default: break;
		}

	double 	mantissa=0.0;
	int 	count=0, pot=0;
	double 	potenza=1.0;
	short 	neg=1;

	for (int i=0; i<buf_len; i++) {
		c=buf[i];
		if ((c!=' ') && (c!='\t')) {
			if (count==-1) return(EASYIO_ERROR);
			else  count++;
			}
		if (count > EASYIO_MAX_FLOAT_LEN) return(EASYIO_ERROR);

		if (c=='-') neg=-1;
		else if ((c >= '0') && (c <= '9')) {
			mantissa=(mantissa*10)+buf[i]-'0';
			if (pot) potenza *= 10;
			}
	   	else if (c=='.') {
			if ((buf[i+1] <'0') || (buf[i+1]>'9')) return(EASYIO_ERROR);
			if (pot != 0) return(EASYIO_ERROR);
			pot=1;
			}
	   	else if ((c==' ') || (c=='\t')) {
			if (count!=0) count=-1;
			}
		else return(EASYIO_ERROR);
		}

	*parm=neg*mantissa/potenza;
	return(EASYIO_OKAY);
	}

// Legge una stringa alfanumerica valida e la copia in parm
int	scanstring (char *parm) {
	int 	c;
	char 	buf[EASYIO_MAX_STR_LEN];
	int 	buf_len=0;

	for (buf_len=0; ((c=getchar()) != '\n') && (c != '#') &&
		(buf_len < EASYIO_MAX_STR_LEN); buf_len++) {
			buf[buf_len]=c;
			// Controllo validità caratteri
			if ((c != '.') && (c < '0') && (c > '9') &&
				(c < 'a') && (c > 'z') && (c < 'A') &&
				(c > 'Z'))  return(EASYIO_ERROR);
			}
	if (c=='#') c=readln();

	switch(buf_len)  {
		case 0: return(EASYIO_DEFAULT);
		case EASYIO_MAX_STR_LEN: return(EASYIO_ERROR);
		default:
			buf[buf_len] = '\0';    // Terminatore stringa
			strcpy(parm, buf);      // Copia in destinazione
			return(EASYIO_OKAY);
		}
	}

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                               easyio_read.Cpp                          */
/*                                                                      */
/*      This module contains procedures which print on the standard     */
/*      output a string containing user prompt and proposed default     */
/*      value.                                                          */
/*      a Read procedure (using the scan functions) is performed; in    */
/*      case of error or out of range value the read procedure is       */
/*      proposed again after a beep. A maximum number of attempts is    */
/*      defined to avoid loops in shells.                               */
/*          read_bool()                                                 */
/*          read_digit()                                                */
/*          read_int()                                                  */
/*          read_double()                                               */
/*          read_string()                                               */
/*                                                                      */
/*----------------------------------------------------------------------*/

#define 	FPTR	stderr  // Output del prompt su stderr per garantire la visibilità anche in caso di reindirizzamenti

//
// read_bool:
//	prompt: prompt user string;
//	def   : proposed default
//
short 	read_bool(char *prompt, short def) {
	int answer;
	for (int timeout = 0; timeout <=5; timeout++)  {
		// Visualizza il prompt con il valore di default
		if (def == false)
			fprintf(FPTR,"%s (y/n) [NO] > ",prompt);
		    else
			fprintf(FPTR,"%s (y/n) [YES] > ",prompt);
		// Legge la risposta dell’utente
		answer = readln();
		switch(answer)  {
			case '\n':  // Nessun input: usa il valore di default
				return(def);
			case 'y':
			case 'Y':
				return(true);  // Conferma positiva
			case 'n':
			case 'N':
				return(false); // Conferma negativa
			default:
				beep();  // Segnala errore e ripete
				break;
			}
		}
	// Troppi tentativi errati: errore fatale
	fprintf(FPTR, "\n FATAL ERROR: too many wrong attempts ");
	fprintf(FPTR, "in read_bool() \n");
	exit(-1);
	return(false); 	    // non viene eseguito: per evitare warning del compilatore
	}

//
// overloaded version without default
//
short 	read_bool(char *prompt) {
	return(read_bool(prompt, true));  // Default implicito: true
	}


/*----------------------------------------------------------------------*/

//
// read_digit:
//	prompt: prompt user string;
//	def   : proposed default
//	min   : inferior limit
//	max   : superior limit
//
int 	read_digit (char *prompt, int def, int min, int max) {
	// Controlli di validità dei limiti
	if (min < 0) min = 0;
	if (max < 0) max = 0;
	if (min > 9) min = 9;
	if (max > 9) max = 9;

	if (min > max) {
		int tmp = min;
		min = max;
		max = tmp;
		}

	if (def < min) def = min;
	if (def > max) def = max;

	short 	esito;
	int 	digit;
	short	failed = 0;

	for (int timeout = 0; timeout <= 5; timeout++)  {
		fprintf(FPTR,"%s ", prompt);
		if (failed) fprintf(FPTR, "(%d::%d) ", min, max);  // Mostra limiti solo dopo un errore
		fprintf(FPTR,"[%d] > ",def);  // Mostra default
		esito = scandigit(&digit);
		switch(esito)  {
			case EASYIO_DEFAULT:
				return(def);  // Nessun input: ritorna default
			case EASYIO_OKAY:
				if ((digit >= min) && (digit <= max))
					return(digit);  // Valore valido
			case EASYIO_ERROR:
				beep();  // Errore: ripeti input
				failed = 1;
				break;
			}
		}
	fprintf(FPTR, "\n FATAL ERROR: too many wrong attempts ");
	fprintf(FPTR, "in read_digit() \n");
	exit(-1);
	return(false);
	}

// Versioni semplificate con meno parametri
int 	read_digit (char *prompt, int def) {
	return(read_digit(prompt, def, 0, 9));
	}
int 	read_digit (char *prompt) {
	return(read_digit(prompt, 0, 0, 9));
	}


/*----------------------------------------------------------------------*/

//
// read_int:
//	prompt: prompt user string;
//	unit  : measurement unit of the reading value
//	def   : proposed default
//	min   : inferior limit
//	max   : superior limit
//

int 	read_int (char *prompt, char *unit, int def, int min, int max) {
        if (min > max) {
		int tmp = min;
	  	min = max;
	  	max = tmp;
		}

        if (def < min) def = min;
	if (def > max) def = max;

	int 	intero;
	short 	esito;
	short	failed = 0;

	for (int timeout = 0; timeout <= 5; timeout++)  {
		// Visualizza prompt, limiti (se errore precedente) e default
		fprintf(FPTR,"%s ", prompt);
		if (failed) fprintf(FPTR, "(%d::%d) ", min, max);
		fprintf(FPTR,"[%d%s] > ", def, unit);
		esito = scanint(&intero);
		switch(esito)  {
			case EASYIO_DEFAULT :
				return(def);
			case EASYIO_OKAY :
				if ((intero >= min) && (intero <= max))
					return(intero);
			case EASYIO_ERROR :
				beep();
				failed = 1;
				break;
			}
		}
	fprintf(FPTR, "\n FATAL ERROR: too many wrong attempts ");
	fprintf(FPTR, "in read_int() \n");
	exit(-1);
	return(false);
	}

// Versioni sovraccaricate con meno argomenti
int 	read_int (char *prompt, int def, int min, int max) {
	return( read_int(prompt, "", def, min, max));
	}
int 	read_int (char *prompt, char *unit, int def) {
	return( read_int(prompt, unit, def, EIO_MIN_INT, EIO_MAX_INT));
	}
int 	read_int (char *prompt, int def) {
	return( read_int(prompt, def, EIO_MIN_INT, EIO_MAX_INT));
	}
int 	read_int (char *prompt) {
	return( read_int(prompt, 0, EIO_MIN_INT, EIO_MAX_INT));
	}


/*----------------------------------------------------------------------*/

//
// read_long:
// Identica a read_int ma per valori long
//
long 	read_long (char *prompt, char *unit, long def, long min, long max) {
	if (min > max) {
		long tmp = min;
		min = max;
		max = tmp;
		}

	if (def < min) def = min;
	if (def > max) def = max;

	long 	intero;
	short 	esito;
	short	failed = 0;

	for (int timeout = 0; timeout <= 5; timeout++)  {
		fprintf(FPTR,"%s ", prompt);
		if (failed) fprintf(FPTR, "(%ld::%ld) ", min, max);
		fprintf(FPTR,"[%ld%s] > ", def, unit);
		esito = scanlong(&intero);
		switch(esito)  {
			case EASYIO_DEFAULT :
				return(def);
			case EASYIO_OKAY :
				if ((intero >= min) && (intero <= max))
					return(intero);
			case EASYIO_ERROR :
				beep();
				failed = 1;
				break;
			}
		}
	fprintf(FPTR, "\n FATAL ERROR: too many wrong attempts ");
	fprintf(FPTR, "in read_long() \n");
	exit(-1);
	return(false);
	}

long 	read_long (char *prompt, long def, long min, long max) {
	return( read_long(prompt, "", def, min, max));
	}
long 	read_long (char *prompt, char *unit, long def) {
	return( read_long(prompt, unit, def, EIO_MIN_INT, EIO_MAX_INT));
	}
long 	read_long (char *prompt, long def) {
	return( read_long(prompt, def, EIO_MIN_INT, EIO_MAX_INT));
	}
long 	read_long (char *prompt) {
	return( read_long(prompt, 0, EIO_MIN_INT, EIO_MAX_INT));
	}

/*----------------------------------------------------------------------*/

//
// read_double:
// Per lettura di numeri in virgola mobile, simile a read_int
//
double 	read_double (char *prompt, char *unit, double def,
			double min, double max) {
	if (min > max) {
		double tmp = min;
		min = max;
		max = tmp;
		}

	if (def < min) def = min;
	if (def > max) def = max;

	double 	doppio;
	short 	esito;
	short	failed = 0;

	for (int timeout = 0; timeout <= 5; timeout++)  {
		fprintf(FPTR,"%s ", prompt);
		if (failed) fprintf(FPTR, "(%.4f::%.4f) ", min, max);
		fprintf(FPTR,"[%.4f%s] > ", def, unit);
		esito = scandouble(&doppio);
		switch(esito)  {
			case EASYIO_DEFAULT :
				return(def);
			case EASYIO_OKAY :
				if ((doppio >= min) && (doppio <= max))
					return(doppio);
			case EASYIO_ERROR :
				beep();
				failed = 1;
				break;
			}
		}
	fprintf(FPTR, "\n FATAL ERROR: too many wrong attempts ");
	fprintf(FPTR, "in read_double() \n");
	exit(-1);
	return(false);
	}

double 	read_double (char *prompt, double def, double min, double max) {
	return( read_double(prompt, "", def, min, max));
	}
double 	read_double (char *prompt, char *unit, double def) {
	return( read_double(prompt, unit, def, EIO_MIN_INT, EIO_MAX_INT));
	}
double 	read_double (char *prompt, double def) {
	return( read_double(prompt, def, EIO_MIN_INT, EIO_MAX_INT));
	}
double 	read_double (char *prompt) {
	return( read_double(prompt, 0, EIO_MIN_INT, EIO_MAX_INT));
	}


/*----------------------------------------------------------------------*/

//
// read_string:
//	prompt: prompt user string;
//	def   : proposed default
//
char * 	read_string (char *prompt, char *def) {
	short 	esito;
	static char 	stringa[EASYIO_MAX_STR_LEN];

	for (int timeout = 0; timeout <= 5; timeout++)  {
		fprintf(FPTR,"%s [%s] > ",prompt,def);
		esito = scanstring(stringa);
		switch(esito)  {
			case EASYIO_DEFAULT:
				return(def);  // Usa il default
			case EASYIO_OKAY:
				return(stringa);  // Restituisce la stringa inserita
			case EASYIO_ERROR:
				beep();  // Errore: ripete il ciclo
				break;
			}
		}
	fprintf(FPTR, "\n FATAL ERROR: too many wrong attempts ");
	fprintf(FPTR, "in read_string() \n");
	exit(-1);
	return(false);
	}

char * 	read_string (char *prompt) {
	return( read_string(prompt, ""));
	}

;

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


;


/***********************************************************************
		EVENT.CPP
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


;


/*******************************************************
EVENT . H
*******************************************************/

#ifndef _EVENT_H
#define _EVENT_H

#include "global.h"
#include "buffer.h"

// Classe base per eventi nella simulazione
class event {
public:
	event*	next;   // puntatore all'evento successivo nella lista eventi
	double 	time;   // tempo in cui si verifica l'evento

	event();                 // costruttore di default
	event(double Time);      // costruttore con tempo specificato
	event(event* Next, double Time); // costruttore con evento successivo e tempo
	~event(){}               // distruttore vuoto (virtuale per eredità)

	// Metodo virtuale da sovrascrivere per definire il comportamento dell'evento
	virtual void body() {}
};

// Implementazioni inline dei costruttori della classe event
inline event::event() {
	next = NULL;
	time = -1;               // tempo di default -1 indica evento non inizializzato
}

inline event::event(event* Next, double Time) {
	next = Next;
	time = Time;
}

inline event::event(double Time) {
	time = Time;
}

// Classe derivata per eventi di arrivo
class arrival : public event {
	buffer* buf;              // buffer associato all'arrivo (dove inserire il pacchetto)

public:
	int source_id;            // id della sorgente del pacchetto/evento
	virtual void body();      // implementazione del comportamento specifico di arrivo
	arrival(double Time, buffer* Buf); // costruttore con tempo e buffer
};

// Classe derivata per eventi di servizio (servizio del buffer)
class service : public event {
	buffer* buf;              // buffer associato al servizio

public:
	virtual void body();      // implementazione del comportamento specifico di servizio
	service(double Time, buffer* Buf) : event(Time) { buf = Buf; }  // costruttore con tempo e buffer
};

// Implementazione inline del costruttore di arrival
inline arrival::arrival(double Time, buffer* Buf) : event(Time) {
	buf = Buf;
}

#endif

;


#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <math.h>    // libreria matematica per funzioni come sqrt, pow, ecc.
#include <stdlib.h>  // libreria standard per funzioni come malloc, free, exit, ecc.
#include <stdio.h>   // libreria standard per input/output (printf, fprintf, ecc.)

// Definizione di una costante SEED usata probabilmente per inizializzare generatori di numeri casuali
#define SEED 2

// Definizione di una costante END che potrebbe indicare un valore di terminazione o limite per un ciclo
#define END 100

#endif


;


/* -*- C++ -*- */
#include "stdio.h"
#include "global.h"
#include "queue.h"
#include "simulator.h"

// Funzione principale: punto di ingresso del programma
int main(int argc, char *argv[])
{
	simulator *eval; // Puntatore alla classe base simulator

	// Pulizia visiva della console con molte righe vuote
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	// Intestazione di benvenuto
	printf("**********************************************************\n\n");
	printf("               G/G/1 QUEUE SIMULATION PROGRAM\n\n");
	printf("**********************************************************\n\n");

	// Creazione di un oggetto queue (che rappresenta il simulatore di coda)
	// La classe queue estende simulator e usa argc/argv per configurare i parametri
	eval = new queue(argc, argv);

	// Inizializzazione della simulazione (lettura parametri, setup iniziale)
	eval->init();

	// Avvio dell'esecuzione della simulazione (gestione eventi)
	eval->run();

	// Calcolo e visualizzazione dei risultati finali
	eval->results();

	// Deallocazione dell'oggetto simulatore per liberare memoria
	delete(eval);
}

;

/***************************************************************************
			PACKET.C
***************************************************************************/

#include "packet.h"


;

/***************************************************************************
PACKET.H
***************************************************************************/

#ifndef PACKET_H
#define PACKET_H

#include "global.h"  // Include definizioni globali e costanti comuni

// Classe packet: rappresenta un pacchetto con un tempo di generazione
class packet {

	double gen_time;  // Tempo in cui il pacchetto è stato generato

public:
	packet(double Gen_time);  // Costruttore che imposta il tempo di generazione
	~packet(){}               // Distruttore vuoto (non fa nulla di particolare)

	packet* next;  // Puntatore al prossimo pacchetto (per liste collegate)

public:
	double get_time(){ return gen_time; }  // Metodo per ottenere il tempo di generazione
};

// Implementazione inline del costruttore
inline packet::packet(double Gen_time){
	gen_time = Gen_time;  // Inizializza il tempo di generazione
	next = NULL;          // Inizializza il puntatore al prossimo pacchetto a NULL
}

#endif

;


/* -*- C++ -*- */
/*******************************************************
		QUEUE.CPP
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

;

/*******************************************************
		     G/G/1 QUEUE SIMULATOR
*******************************************************/
#ifndef _QUEUE_H
#define _QUEUE_H

#include "simulator.h"   // Classe base per la simulazione
#include "calendar.h"    // Gestione eventi nel calendario
#include "event.h"       // Definizione degli eventi
#include "buffer.h"      // Gestione del buffer della coda
#include "packet.h"      // Definizione del pacchetto
#include "stat.h"        // Statistiche per le misure

// Classe queue che estende simulator per simulare una coda G/G/1
class queue: public simulator{

	virtual void input(void);     // Metodo per gestire input specifico

	buffer* buf;                  // Buffer della coda (lista pacchetti)
	int traffic_model;            // Modello del traffico (es. distribuzione arrivi)
	double load;                  // Carico della coda (intensità traffico)
	int service_model;            // Modello di servizio (es. distribuzione tempi di servizio)

	// Contatori per la simulazione
	double packets;               // Numero di pacchetti processati
	double tot_delay;             // Ritardo totale accumulato

	// Statistiche sul ritardo
	Sstat* delay;                 // Statistiche per i ritardi dei pacchetti

public:
	queue(int argc,char *argv[]); // Costruttore con argomenti da linea di comando
	virtual ~queue(void);         // Distruttore virtuale

	virtual void init(void);      // Inizializza la simulazione
	virtual void run(void);       // Esegue la simulazione

private:
	virtual void clear_counters(void);   // Pulisce i contatori prima di una nuova simulazione
	virtual void clear_stats(void);      // Pulisce le statistiche raccolte
	virtual void update_stats(void);     // Aggiorna le statistiche dopo un evento
	virtual void print_trace(int Run);   // Stampa il tracciato/log della simulazione per un run specifico
	virtual void results(void);           // Mostra i risultati finali della simulazione
	virtual int isconfsatisf(double perc); // Controlla se la confidenza statistica è soddisfatta (perc = percentuale)
};

#endif


;


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


;


/* -*- C++ -*- */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*		<<<NUOVO>>>	    R A N D . H				*/
/*                                                                      */
/*            MODULO DI GENERAZIONE EVENTI CASUALI                      */
/*                                                                      */
/*     Questo modulo contiene le macro per la generazione di eventi     */
/*     pseudocasuali:                                                   */
/*            - RESET_SEED : prepara i semi della routine PSEUDO        */
/*            - PSEUDO     : genera un numero pseudocasuale compreso    */
/*                           tra 0 e 1                                  */
/*	      - GEN_UNIF   : genera un numero intero compreso tra	*/
/*			     min e max					*/
/*	      - GEN_POISSON: genera un evento di poisson		*/
/*	      - GEN_BERNOULLI: genera un numero con prob. bernoulliana  */
/*	      - GEN_EXP    : genera un esponenziale			*/
/*	      - GEN_EXP_INT: genera un exp e lo approssima con un int	*/ 
/*	      - GEN_EXP_ONE: genera un exp intero di valore minimo 1    */
/*                                                                      */
/*----------------------------------------------------------------------*/
#ifndef _RAND_H
#define _RAND_H

#include <math.h>
#include "global.h"

/*
 * Vettore contenente i semi per la generazione di numeri casuali.
 * Viene inizializzato in rand.c oppure con la macro RESET_SEED.
 */
extern	int	lseed[11];

/*
 * Macro di inizializzazione dei semi di generazione.
 * Assegna valori fissi al vettore lseed, pronti per la generazione casuale.
 */
#define RESET_SEED		\
{				\
lseed[0] = 0;			\
lseed[1] = 428956419;		\
lseed[2] = 1954324947;		\
lseed[3] = 1145661099;		\
lseed[4] = 1835732737;		\
lseed[5] = 794161987;		\
lseed[6] = 1329531353;		\
lseed[7] = 200496737;		\
lseed[8] = 633816299;		\
lseed[9] = 1410143363;		\
lseed[10] =1282538739;		\
}

/*
 * Macro per generare un numero pseudocasuale double compreso tra 0 e 1,
 * usando il seme di indice IS (1..10).
 * Assegna il risultato alla variabile PSEUDONUM.
 */
#define	la  16807
#define	lb15  32768
#define	lb16  65536
#define	lp  2147483647
#define PSEUDO(IS,PSEUDONUM)                                     \
{                                                                \
int     lfhi,lhi,lalo,leftlo,_k,iseed;				 \
double  _x;							 \
	iseed = lseed [IS];                                      \
	lhi = iseed/lb16;                                        \
	lalo = (iseed-lhi*lb16)*la;                              \
	leftlo = lalo/lb16;                                      \
	lfhi = lhi*la + leftlo;                                  \
	_k = lfhi/lb15;                                          \
	iseed = (((lalo-leftlo*lb16)-lp)+(lfhi-_k*lb15)*lb16)+_k;\
	while (iseed < 0)                                        \
		iseed += lp;                                     \
	lseed[IS] = iseed;                                       \
	_x = lseed[IS];                                           \
	PSEUDONUM=(_x*4.656612875e-10);                           \
}

/*
 * Macro per generare un numero intero uniforme compreso tra Min e Max,
 * usando il seme IS. Il risultato viene salvato in ESITO.
 */
#define		GEN_UNIF(IS, Min, Max, ESITO)			\
	{							\
	double pseudonum;					\
	PSEUDO((IS), pseudonum);				\
	ESITO = (int)((Min) + pseudonum * (fabs((double)((Max) - (Min)))+1));\
	}

/*
 * Macro per generare un evento di Poisson, con probabilità PROB.
 * Restituisce 0 o 1 in ESITO a seconda se l'evento si verifica.
 */
#define 	GEN_POISSON(IS, PROB, ESITO)		\
	{						\
	double pseudonum;				\
	PSEUDO((IS),pseudonum);				\
	if (pseudonum > (PROB)) ESITO=0;		\
	           else ESITO=1;			\
	}

/*
 * Macro per generare un numero con distribuzione Bernoulliana,
 * sommando fino a TOP eventi di Poisson con probabilità PROB.
 */
#define		GEN_BERNOULLI(IS, TOP, PROB, ESITO)	\
	{						\
	int NUM;        				\
	int i;						\
	ESITO = 0;					\
	for(i=0; i<TOP; i++)				\
           {						\
	   GEN_POISSON(IS, PROB, NUM);			\
	   ESITO += NUM;				\
	   }						\
	}

/*
 * Macro per generare un numero double esponenziale con media AVERAGE.
 */
#define		GEN_EXP(IS, AVERAGE, RES)		\
	{						\
        double pseudonum;				\
        PSEUDO(IS,pseudonum);				\
        RES = -log(pseudonum) * (double)(AVERAGE);	\
  	}

/*
 * Macro per generare un intero con distribuzione esponenziale,
 * approssimando il valore double e arrotondando.
 */
#define 	GEN_EXP_INT(IS, AVERAGE, RES)		\
	{						\
	double expnum;					\
	GEN_EXP(IS, AVERAGE, expnum);			\
	RES = (int)(expnum + 0.5);			\
	if (RES < 0) RES=0;				\
	}

/*
 * Macro per generare un intero esponenziale con valore minimo 1.
 */
#define 	GEN_EXP_ONE(IS, AVERAGE, RES)		\
	{						\
	double expnum;					\
	GEN_EXP(IS, (AVERAGE-0.5), expnum);		\
	RES = (int)(expnum + 1);			\
	if (RES < 0) RES=0;				\
	}

/* Funzioni dichiarate (definite altrove) */
int arrotonda(double x);
int transmit(double g);
int fact(int i);
int n_interf_by_poisson(double average);

#endif	// _MACRO_RANDOM_H


;


/* -*- C++ -*- */
/*******************************************************
		SIMULATOR.CPP
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


;

/* -*- C++ -*- */
/********************************************************
		SIMULATOR.H
	Classe astratta per programmi di simulazione

*******************************************************/

#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <stdio.h>

class simulator {
protected:
	FILE *fptrc;   // file pointer per il trace (log di eventi)
	FILE *fpout;   // file pointer per l'output dei risultati

	void read_args(int argc, char* argv[]);  // metodo per leggere argomenti da riga di comando

	// Metodi astratti che devono essere implementati nelle classi derivate
	virtual void input(void) = 0;         // metodo per leggere input specifico della simulazione
	virtual void print_trace(int i) = 0; // metodo per stampare il trace/log

public:
	simulator(int argc, char* argv[]);  // costruttore con argomenti da linea di comando
	virtual ~simulator(void) {;}        // distruttore virtuale (vuoto)

	// Metodi astratti pubblici che definiscono il ciclo della simulazione
	virtual void init(void) = 0;    // inizializza la simulazione
	virtual void run(void) = 0;     // esegue la simulazione
	virtual void results(void) = 0; // mostra i risultati
};

#endif


;

/*****************************************************************************

			       S T A T . CPP

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


;

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

;

