/* -*- C++ -*- */
/*******************************************************
		SIMULATOR.C
	Abstract class for simulation program

*******************************************************/

#include <stdio.h>
#include "simulator.h"
#include <stdlib.h>

inline void GoFatal() {
        fprintf(stderr, "usage: -o output_file -t trace_file\n");
        exit(-1);
        }

inline  char read_opt(char* s) {
                if (s[0] != '-')  GoFatal();
                if (s[2] != '\0') GoFatal();
                return s[1];
                }


void simulator::read_args(int argc,char* argv[])
{
        int     f_o = -1;
        int     f_t = -1;
        char    c;
        int     base = 1;
        argc--;
        while (argc > 0) {
                c = read_opt(argv[base]);
                argc--;
                base++;
                switch (c) {
                        case 'o': if (f_o != -1) GoFatal();
                                  if (argc == 0) GoFatal();
                                  fpout = fopen(argv[base], "w");
                                  argc--; base++;
                                  f_o = 1;
                                  break;

                        case 't': if (f_t != -1) GoFatal();
                                  if (argc == 0) GoFatal();
                                  fptrc = fopen(argv[base], "w");
                                  argc--; base++;
                                  f_t = 1;
                                  break;
                        default : GoFatal();
                        }
                }

        if (f_o == -1) fpout= stdout;
        if (f_t == -1) fptrc = fpout;

}

simulator::simulator(int argc,char* argv[])
{
  fptrc=NULL;
  fpout=NULL;
  read_args(argc,argv);
}

