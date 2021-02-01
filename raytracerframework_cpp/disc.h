#pragma once

#include "object.h"

class Disc : public Object
{
public:
    Disc(Point position, Triple rotation, Triple velocity, long double r) : Object(position, rotation, velocity), r(r) { }

    virtual Hit intersect(const Ray &ray);
	Point getHit(double u, double v);

    const long double r;
};
