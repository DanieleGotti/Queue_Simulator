/*****************************************************************************

				S T A T . H

	Contains definition of the following two classes:
	      - Sstat: 	Modified and extended from class SampleStatistic of 
			GNU library. 

 *****************************************************************************/
#ifndef _STAT_H
#define _STAT_H

#include <math.h>

//----------------------------------------------------------------------------
//				CLASS Sstat
//----------------------------------------------------------------------------

class 	Sstat 	{
	protected:
    		int	n;
    		double 	x;
    		double 	x2;
		double  last;
   		double 	minValue;
		double	maxValue;
    	public :
    		Sstat();
    		virtual ~Sstat();
    		virtual void reset(); 
    		virtual void operator+=(double);
    		int  	num_samples();
		double	last_sample();
   		double  sum();
    		double 	mean();
    		double 	stddev();
    		double 	var();
    		double 	min();
    		double 	max();
    		double 	confidence(int p_percentage);
    		double 	confidence(double p_value);
    		double 	confpercerr(int p_percentage);
    		double 	confpercerr(double p_value);
		int	isconfsatisfied(double perc=1.0, double pconf=.95);
};


inline 		Sstat:: Sstat() {reset();}
inline 		Sstat::~Sstat() {}

inline 	int 	Sstat::num_samples() {return(n);}
inline	double	Sstat::last_sample() {return(last);}
inline 	double 	Sstat::min() {return(minValue);}
inline 	double 	Sstat::max() {return(maxValue);}
inline 	double 	Sstat::sum() {return(x);}

inline  double	Sstat::confidence(int p_percentage) { 
			return confidence(double(p_percentage)*0.01); }

inline  double	Sstat::confpercerr(int p_percentage) { 
			return confpercerr(double(p_percentage)*0.01); }

inline  int   	Sstat::isconfsatisfied(double perc, double pconf) {
	return (confpercerr(pconf) < perc); }


#endif	/* _STAT_H */
