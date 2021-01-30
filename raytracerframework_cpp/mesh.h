#pragma once

#include "object.h"
#include "glm.h"

class Mesh : public Object
{
	GLMmodel* model;


	Mesh(Point position, Triple rotation, Triple velocity, char* filename) : Object(position, rotation, velocity) 
	{
		model = glmReadOBJ(filename);
	}

	virtual Hit intersect(const Ray &ray);
	Vector getUV(Point hit, Vector n);
	Point getHit(double u, double v);
};

