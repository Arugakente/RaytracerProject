#pragma once

#include "object.h"

class Plane : public Object
{
public:
	Plane(Point position, Triple rotation, Triple velocity,long double h,long double w) : Object(position, rotation, velocity),height(h),width(w) {}

	virtual Hit intersect(const Ray &ray);

	Point getHit(double u, double v);
	long double height;
	long double width;
};