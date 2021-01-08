//    
//  Students:
//    Guillaume Gosset
//    Quentin Lasota
//

#pragma once

//constants from computation library
#ifndef M_PI
#define M_PI          3.14159265358979323846
#endif

/* epsilon surrounding for near zero values */

#define     EQN_EPS     1e-9
#define	    IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)

#ifdef NOCBRT
#define     cbrt(x)     ((x) > 0.0 ? pow((long double)(x), 1.0/3.0) : \
                          ((x) < 0.0 ? -pow((long double)-(x), 1.0/3.0) : 0.0))
#endif

int SolveQuadric(long double c[ 3 ],long double s[ 2 ]);
int SolveCubic(long double c [ 4 ],long double s[ 3 ]);
int SolveQuartic(long double c[ 5 ],long double s[ 4 ]);