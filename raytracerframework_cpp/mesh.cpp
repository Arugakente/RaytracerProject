#include "mesh.h"
#include <iostream>
#include <math.h>

Hit Mesh::intersect(const Ray &ray)
{
	Hit res = Hit(std::numeric_limits<double>::infinity(), Vector(0, 0, 0));
	bool hasIntersected = false;

	for (Triangle t : triangles) {

		Hit h = t.intersect(ray);

		if (h.t < res.t)
		{
			if (!hasIntersected) hasIntersected = true;
			res = h;
		}
	}
	if (!hasIntersected) return Hit::NO_HIT();

	return res;
}

Vector Mesh::getUV(Point hit, Vector n)
{
	return Vector(0, 0, 0);
}


Point Mesh::getHit(double u, double v)
{
	return Point(0, 0, 0);
}