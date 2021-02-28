#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Cone **********************************/

Hit Triangle::intersect(const Ray &ray)
{
	Ray TransformedRay = transform(ray);

    Vector CO = -TransformedRay.O; //C - ray.O with C = (0,0,0)
	Vector N = (v2Offset-v1Offset).cross(v3Offset-v1Offset);
	N.normalize();

	if(N.dot(TransformedRay.D) == 0)
		return Hit::NO_HIT();

	long double t = (CO.dot(N) / N.dot(TransformedRay.D));

	if (t < 0) return Hit::NO_HIT();

	Vector intersect = TransformedRay.O + t * TransformedRay.D;

	//check if inside the triangle
	Vector e1 = v2Offset-v1Offset;
	Vector e2 = v3Offset-v2Offset;
	Vector e3 = v1Offset-v3Offset;

	Vector C0 = intersect - v1Offset;
	Vector C1 = intersect - v2Offset;
	Vector C2 = intersect - v3Offset;

	if (N.dot(e1.cross(C0)) <= 0 ||
    	N.dot(e2.cross(C1)) <= 0 ||
    	N.dot(e3.cross(C2)) <= 0 )
		return Hit::NO_HIT();

	//std::cout << intersect << std::endl;
	//std::cout << TransformedRay.O << std::endl;
	//std::cout << TransformedRay.D << std::endl;
	//std::cout << abs(intersect.dot(N)) << std::endl << std::endl;

	N = removeTransformation(N);
	return Hit(t, N);
}

Point Triangle::getHit(double u, double v)
{
	return Point(0, 0, 0);
}