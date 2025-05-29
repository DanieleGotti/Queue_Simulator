/************************************************************************
	THIS FILE COLLAPSES THREE MODULES, FORMERLY CALLED 
	EASYIO_PRIM.C, EASYIO_SCAN.C and EASYIO_READ.C
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "easyio.h"

/*----------------------------------------------------------------------*/
/*                                                                      */
/*                               easyio_prim.C                          */
/*                                                                      */
/*      This module contains some procedures which help the input       */
/*      data process for an user program.                               */
/*      Function contained are the following:                           */
/*           - clear_screen : clear a page of the screen                */
/*           - readln       : read the first character ignoring the     */
/*			      remaining input stream until the          */
/*			      following <CR>                            */
/*           - beep         : beeps                                     */
/*           - pausa        : waits the user to press <CR>              */  
/*                                                                      */
/*----------------------------------------------------------------------*/


void clear_screen() {
        system("clear");
	return;
	}


char readln() {
	char d = getchar();
	char c = d;
	while (d!='\n')
		d=getchar();
	return (c);
	}


void beep() {
	putchar(7);
	return;
	}


void pausa() {
	fprintf(stdout,"\n\n");
        fprintf(stdout,"     *****   Press <RETURN> to continue   *****");
	readln();
	clear_screen();
	return;
	}


/*----------------------------------------------------------------------*/
/*                                                                      */
/*                               easyio_scan.C                          */
/*                                                                      */
/*                                                                      */
/*      This module contains procedures similar to the scanf() function:*/
/*      a variable is read from the standard input; once a valid value  */
/*      is given, this value is stored in the 'parm' pointed location,  */
/*      otherwise the parm pointer is unchanged.                        */
/*      Functions:                                                      */
/*         scandigit()                                                  */
/*         scanint()                                                    */
/*         scandouble()                                                 */
/*         scanstring()                                                 */
/*      Return values:                                                  */
/*         - EASYIO_OKAY    : the value is valid;                       */
/*         - EASYIO_DEFAULT : the first character in input is <CR>;     */
/*         - EASYIO_ERROR   : error detected: string too long or        */
/*			      non valid characters included.            */
/*      As begin comment / EOL character, '#' is used.                  */
/*                                                                      */
/*----------------------------------------------------------------------*/

//
// scandigit: max string length: 1 character;
//            valid characters: '0' to '9'
//

int	scandigit (int *parm) {
	int	c = readln();
	switch (c)  {
		case '\n' :
			return(EASYIO_DEFAULT);
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			*parm = c -'0';
			return(EASYIO_OKAY);
		default:
			return(EASYIO_ERROR);
		}
	}


/*----------------------------------------------------------------------*/



//
// scanint:   max string length: EASYIO_MAX_INT_LEN;
//            valid characters: '0'-'9'
//

int	scanint (int *parm){

	int 	c;
	int	buf[EASYIO_MAX_BUF_LEN];
	int 	buf_len;

	for (buf_len=0; ((c=getchar()) != '\n') && (c != '#') && 
		(buf_len < EASYIO_MAX_BUF_LEN); buf_len++) buf[buf_len]=c;
	if (c=='#') c=readln();	/* empty buffer for next line */

	switch (buf_len)  {
		case 0:
			return(EASYIO_DEFAULT);
		case EASYIO_MAX_BUF_LEN:
			return(EASYIO_ERROR);
		default:
			break;
		}

	int 	intero=0;
	int 	count=0;
	short 	neg=1;

	for (int i=0; i<buf_len; i++) {
		c=buf[i];
		if ((c!=' ') && (c!='\t')) {
			if (count==-1) return(EASYIO_ERROR);
		      	else  count++;
			}
		if (count > EASYIO_MAX_INT_LEN) 
			return(EASYIO_ERROR);
		if (c=='-') 
			neg=-1;
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


/*----------------------------------------------------------------------*/


//
// scanlong:   max string length: EASYIO_MAX_INT_LEN;
//            valid characters: '0'-'9'
//

int	scanlong (long *parm){

	int 	c;
	int	buf[EASYIO_MAX_BUF_LEN];
	int 	buf_len;

	for (buf_len=0; ((c=getchar()) != '\n') && (c != '#') &&
		(buf_len < EASYIO_MAX_BUF_LEN); buf_len++) buf[buf_len]=c;
	if (c=='#') c=readln();	/* empty buffer for next line */

	switch (buf_len)  {
		case 0:
			return(EASYIO_DEFAULT);
		case EASYIO_MAX_BUF_LEN:
			return(EASYIO_ERROR);
		default:
			break;
		}

	long 	intero=0;
	int 	count=0;
	short 	neg=1;

	for (int i=0; i<buf_len; i++) {
		c=buf[i];
		if ((c!=' ') && (c!='\t')) {
			if (count==-1) return(EASYIO_ERROR);
					else  count++;
			}
		if (count > EASYIO_MAX_INT_LEN)
			return(EASYIO_ERROR);
		if (c=='-')
			neg=-1;
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


/*----------------------------------------------------------------------*/


//
// scandouble:   max string length: EASYIO_MAX_FLOAT_LEN;
//            valid characters: '0'-'9', '.'
//

int	scandouble (double *parm) {

	int 	c;
	int 	buf_len;
	int	buf[EASYIO_MAX_BUF_LEN];


	for (buf_len=0; ((c=getchar()) != '\n') && (c != '#') && 
		(buf_len < EASYIO_MAX_BUF_LEN); buf_len++) buf[buf_len]=c;
	if (c=='#') c=readln();		/* empty buffer for next line */

	switch (buf_len)  {
		case 0:
			return(EASYIO_DEFAULT);
		case EASYIO_MAX_BUF_LEN:
			return(EASYIO_ERROR);
		default:
			break;
		}

	double 	mantissa=0.0;
	int 	count=0;
	int 	pot=0;
	double 	potenza=1.0;
	short 	neg=1;

	for (int i=0; i<buf_len; i++) {
		c=buf[i];
		if ((c!=' ') && (c!='\t')) {
			if (count==-1) return(EASYIO_ERROR);
		      	else  count++;
			}
		if (count > EASYIO_MAX_FLOAT_LEN) 
			return(EASYIO_ERROR);
		if (c=='-') 
			neg=-1;
		else if ((c >= '0') && (c <= '9')) {
			mantissa=(mantissa*10)+buf[i]-'0';
			if (pot) potenza *= 10;
			}
	   	else if (c=='.') {
			if ((buf[i+1] <'0') || (buf[i+1]>'9'))
			   	return(EASYIO_ERROR);
			if (pot != 0)
			   	return(EASYIO_ERROR);
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


/*----------------------------------------------------------------------*/


//
// scanstring:   max string length: EASYIO_MAX_STR_LEN;
//            valid characters:  '0'-'9','.','a'-'z','A','Z'
//

int	scanstring (char *parm) {
	int 	c;
	char 	buf[EASYIO_MAX_STR_LEN];
	int 	buf_len=0;

	for (buf_len=0; ((c=getchar()) != '\n') && (c != '#') && 
		(buf_len < EASYIO_MAX_STR_LEN); buf_len++) { 
			buf[buf_len]=c;
			if ((c != '.') && (c < '0') && (c > '9') && 
				(c < 'a') && (c > 'z') && (c < 'A') && 
				(c > 'Z'))  return(EASYIO_ERROR);
			}
	if (c=='#') c=readln();	/* empty buffer for next line */

	switch(buf_len)  {
		case 0:
			return(EASYIO_DEFAULT);
		case EASYIO_MAX_STR_LEN:
			return(EASYIO_ERROR);
		default:
			buf[buf_len] = '\0';
			strcpy(parm, buf);
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
#define 	FPTR	stderr


//
// read_bool:
//	prompt: prompt user string;
//	def   : proposed default
//

short 	read_bool(char *prompt, short def) {
	int answer;
	for (int timeout = 0; timeout <=5; timeout++)  {
		if (def == false)
			fprintf(FPTR,"%s (y/n) [NO] > ",prompt);
		    else
			fprintf(FPTR,"%s (y/n) [YES] > ",prompt);
		answer = readln();
		switch(answer)  {
			case '\n':
				return(def);
			case 'y':
			case 'Y':
				return(true);
			case 'n':
			case 'N':
				return(false);
			default:
				beep();
				break;
			}
		}
	fprintf(FPTR, "\n FATAL ERROR: too many wrong attempts ");
	fprintf(FPTR, "in read_bool() \n");
	exit(-1);
	return(false); 	    // not used: just to be nice with the compiler
	}



//
// overloaded version without default
//
short 	read_bool(char *prompt) {
	return(read_bool(prompt, true));
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
		if (failed) fprintf(FPTR, "(%d::%d) ", min, max);
		fprintf(FPTR,"[%d] > ",def);
		esito = scandigit(&digit);
		switch(esito)  {
			case EASYIO_DEFAULT:
				return(def);
			case EASYIO_OKAY:
				if ((digit >= min) && (digit <= max))
					return(digit);
			case EASYIO_ERROR:
				beep();
				failed = 1;
				break;
			}
		}
	fprintf(FPTR, "\n FATAL ERROR: too many wrong attempts ");
	fprintf(FPTR, "in read_digit() \n");
	exit(-1);
	return(false); 	    // not used: just to be nice with the compiler
	}


//
// overloaded version with no check
//
int 	read_digit (char *prompt, int def) {
	return(read_digit(prompt, def, 0, 9));
	}

//
// overloaded version with no check and default
//
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
	return(false); 	    // not used: just to be nice with the compiler
	}


//
// overloading without unit and/or range
//
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
//	prompt: prompt user string;
//	unit  : measurement unit of the reading value
//	def   : proposed default
//	min   : inferior limit
//	max   : superior limit
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
	return(false); 	    // not used: just to be nice with the compiler
	}


//
// overloading without unit and/or range
//
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
//	prompt: prompt user string;
//	unit  : measurement unit of the reading value
//	def   : proposed default
//	min   : inferior limit
//	max   : superior limit
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
	return(false); 	    // not used: just to be nice with the compiler
	}

//
// overloading without unit and/or range

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
				return(def);
			case EASYIO_OKAY:
				return(stringa);
			case EASYIO_ERROR:
				beep();
				break;
			}
		}
	fprintf(FPTR, "\n FATAL ERROR: too many wrong attempts ");
	fprintf(FPTR, "in read_string() \n");
	exit(-1);
	return(false); 	    // not used: just to be nice with the compiler
	}


//
// overloading without default
//
char * 	read_string (char *prompt) {
	return( read_string(prompt, ""));
	}

