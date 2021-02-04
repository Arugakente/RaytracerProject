
#include "disc.h"
#include <iostream>
#include <math.h>

/************************** Cone **********************************/

Hit Disc::intersect(const Ray &ray)
{
	Ray TransformedRay = transform(ray);

	Vector CO = -TransformedRay.O; //C - ray.O with C = (0,0,0)
	Vector N = Vector(0.0,0.0,1.0);

	//if parallel to circle,prevent 0 division.
	if(N.dot(TransformedRay.D) == 0)
		return Hit::NO_HIT();

	long double t = (CO.dot(N) / N.dot(TransformedRay.D));

	if (t < 0) return Hit::NO_HIT();

	Vector intersect = TransformedRay.O + t * TransformedRay.D;
	long double distToCenter = sqrt(intersect.dot(intersect));

	if (distToCenter > r) return Hit::NO_HIT();

	N = removeTransformation(N);
	return Hit(t,N);
}

Point Disc::getHit(double u, double v)
{
	return Point(0, 0, 0);
}
