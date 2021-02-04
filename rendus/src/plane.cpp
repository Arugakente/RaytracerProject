
#include "plane.h"
#include <iostream>
#include <math.h>

/************************** Cone **********************************/

Hit Plane::intersect(const Ray &ray)
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

	if(width != -1 && abs(intersect.x)>=width/2)
		return Hit::NO_HIT();

	if(height != -1 & abs(intersect.y)>=height/2)
		return Hit::NO_HIT();
	N = removeTransformation(N);
	return Hit(t,N);
}

Vector Plane::getUV(Point hit, Vector n)
{
	if(height == -1 || width == -1)
		return Vector(0,0);
	Point absolute = hit-position;
	return Vector((absolute.x+width/2)/width,(absolute.y+height/2)/height);
}

Point Plane::getHit(double u, double v)
{
	return Point(0, 0, 0);
}
