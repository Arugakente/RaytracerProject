//
//  Framework for a raytracer
//  File: torus.h
//  Extended from sphere.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//    
//  Students:
//    Guillaume Gosset
//    Quentin Lasota
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef TORUS_H_115209AE
#define TORUS_H_115209AE

#include "object.h"

class Torus : public Object
{
public:
    Torus(Point position, Triple rotation, Triple velocity, long double r,long double R) : Object(position, rotation, velocity), r(r), R(R) { }

    virtual Hit intersect(const Ray &ray);
	Point getHit(double u, double v);

    const long double r;
    const long double R;
};

#endif /* end of include guard: TORUS_H_115209AE */
