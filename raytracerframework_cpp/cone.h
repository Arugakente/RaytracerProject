//
//  Framework for a raytracer
//  File: cone.h
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

#pragma once

#include "object.h"

class Cone : public Object
{
public:
	Cone(Point position, Triple rotation, long double r, long double h) : Object(position, rotation), r(r), h(h) { }

	virtual Hit intersect(const Ray &ray);
	long double solveDisc(const Ray &ray, Vector V);

	const long double r;
	const long double h;
};

