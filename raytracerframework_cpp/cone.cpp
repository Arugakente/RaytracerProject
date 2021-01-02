//
//  Framework for a raytracer
//  File: cone.cpp
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

#include "cone.h"
#include <iostream>
#include <math.h>

/************************** Cone **********************************/

Hit Cone::intersect(const Ray &ray)
{
	Ray TransformedRay = ray;

	Vector V = Vector(0, -h, 0);
	V.normalize();
	Point C = position + Point(0, h, 0);
	Vector CO = (Vector)(TransformedRay.O - C);

	double DdotV = TransformedRay.D.dot(V);
	double COdotV = CO.dot(V);
	double theta = atan(r / h);
	double cosThetaSquared = cos(theta) * cos(theta);

	//second order equation solving
	double a = DdotV * DdotV - cosThetaSquared;
	double b = 2 * (DdotV * COdotV - TransformedRay.D.dot(CO) * cosThetaSquared);
	double c = COdotV * COdotV - CO.dot(CO) * cosThetaSquared;

	double disc = b * b - 4.0*a*c;

	if (disc < 0.0) {
		return Hit::NO_HIT();
	}
	float t1 = (-b - disc) / (2.0*a);
	float t2 = (-b + disc) / (2.0*a);

	float t = t1;
	if (t < 0.0 || t2 > 0.0 && t2 < t) t = t2;
	if (t < 0.0) {
		return Hit::NO_HIT();
	}

	Point intersect = TransformedRay.O + t * TransformedRay.D;
	Vector CP = ((Vector)(intersect - C));
	double h_intersect = CP.dot(V);

	//std:cout << h_intersect << std::endl;

	/*if (h_intersect < 0. || h_intersect > h) {
		return Hit::NO_HIT();
	}*/

	Vector N = (CP * V.dot(CP) / CP.dot(CP) - V);
	N.normalize();

	return Hit(t, N);
}
