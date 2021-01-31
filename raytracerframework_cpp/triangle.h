#pragma once

#include "object.h"

class Triangle : public Object
{
public:
	Triangle(Point position, Triple rotation, Triple velocity,Vector v1O,Vector v2O,Vector v3O) : Object(position, rotation, velocity), v1Offset(v1O), v2Offset(v2O), v3Offset(v3O) {}

	virtual Hit intersect(const Ray &ray);

	Point getHit(double u, double v);

	Vector v1Offset; //upper
	Vector v2Offset; //lower right
	Vector v3Offset; //lower left
};