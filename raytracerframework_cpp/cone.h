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
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#pragma once

#include "object.h"

class Cone : public Object
{
public:
	Cone(Point position, Triple rotation, double r, double h) : Object(position, rotation), r(r), h(h) { }

	virtual Hit intersect(const Ray &ray);

	const double r;
	const double h;
};

