/************************************************************************
	THIS FILE COLLAPSES THREE MODULES, FORMERLY CALLED
	EASYIO_PRIM.C, EASYIO_SCAN.C and EASYIO_READ.C
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "easyio.h"

/*----------------------------------------------------------------------*/
/* easyio_prim.C                                                        */
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
/* easyio_scan.C                                                        */
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
/*                               easyio_read.C                          */
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
