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
    Torus(Point position,Triple rotation ,double r,double R) : Object(position,rotation), r(r), R(R) { }

    virtual Hit intersect(const Ray &ray);

    const double r;
    const double R;
};

#endif /* end of include guard: TORUS_H_115209AE */
