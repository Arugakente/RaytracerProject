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
	Ray TransformedRay = transform(ray);

	Vector V = Vector(0, -h, 0);
	V.normalize();
	Point C = Point(0, h, 0); //translation handled by transform()
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
	float t1 = (-b - sqrt(disc)) / (2.0*a);
	float t2 = (-b + sqrt(disc)) / (2.0*a);

	float t3 = solveDisc(TransformedRay, V); //disc handling

	float t = t1;
	if (t < 0.0 || (t2 > 0.0 && t2 < t)) t = t2;
	if (t < 0.0) {
		return Hit::NO_HIT();
	}

	Point intersect = TransformedRay.O + t * TransformedRay.D;
	Vector CP = ((Vector)(intersect - C));
	double h_intersect = CP.dot(V);

	if (h_intersect < 0.) {

		if (t == t1 && t2 > 0.0)
			t = t2;
		else if (t == t2 && t1 > 0.0) {
			t = t1;
		}

		intersect = TransformedRay.O + t * TransformedRay.D;
		CP = ((Vector)(intersect - C));
		h_intersect = CP.dot(V);
	}

	if (h_intersect < 0. || h_intersect > h) {
		if (t3 > 0)
		{
			std::cout << "test" << std::endl;
			t = t3;
			return Hit(t, removeTransformation(V));
		}
		return Hit::NO_HIT();
	}

	Vector N = (CP * V.dot(CP) / CP.dot(CP) - V);
	N = removeTransformation(N);
	N.normalize();

	return Hit(t, N);
}

double Cone::solveDisc(const Ray &ray, Vector V) {

	Vector CO = -ray.O; //C - ray.O with C = (0,0,0)
	Vector N = -V;

	float t = (CO.dot(N) / N.dot(ray.D));

	if (t < 0) return -1.0;

	Vector intersect = ray.O + t * ray.D;
	double distToCenter = sqrt(intersect.dot(intersect));

	if (distToCenter > r) return -1.0;

	return t;
}
