#pragma once

#include "object.h"

class Plane : public Object
{
public:
	Plane(Point position, Triple rotation, Triple velocity) : Object(position, rotation, velocity) {}

	virtual Hit intersect(const Ray &ray);

	Point getHit(double u, double v);
};