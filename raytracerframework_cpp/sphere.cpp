//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "sphere.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return Hit::NO_HIT().
    * Otherwise, return an instance of Hit() with the distance of the
    * intersection point from the ray origin as t and the normal ad N (see example).
    ****************************************************/

    Ray TransformedRay = transform(ray);

	//second order equation solving
	long double a = TransformedRay.D.dot(TransformedRay.D);
	long double b = 2.0 * TransformedRay.O.dot(TransformedRay.D);
	long double c = TransformedRay.O.dot(TransformedRay.O) - r*r;


	long double disc = b*b - 4.0*a*c;

	if (disc < 0) {
		return Hit::NO_HIT();
	}
	
    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    * 
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

    double t;
    double t1 = (-b - sqrt(disc)) / (2.0*a);
    double t2 = (-b + sqrt(disc)) / (2.0*a);

    if (t1 < t2 && t1 > 0.0) t = t1;
    else {
        if (t2 > 0.0) t = t2;
        else return Hit::NO_HIT();
    }

	Point intersect = ray.O + t * ray.D;
	
	Vector N = (Vector) (intersect - position);
	N.normalize();
    
    return Hit(t,N);
}
