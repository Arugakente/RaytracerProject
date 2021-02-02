#pragma once

#include "object.h"
#include "glm.h"

class Mesh : public Object
{
public:
	GLMmodel* model;

	Mesh(Point position, Triple rotation, Triple velocity, char* objfile) : Object(position, rotation, velocity) 
	{
		model = glmReadOBJ(objfile);
		std::cout << model->numvertices << std::endl;
		std::cout << position << std::endl;
	}


	virtual Hit intersect(const Ray &ray);
	Vector getUV(Point hit, Vector n);
	Point getHit(double u, double v);
};

