#pragma once

#include "object.h"
#include "glm.h"

class Mesh : public Object
{
public:
	GLMmodel* model;
	                                                                       //could be used
	Mesh(Point position, Triple rotation, Triple velocity, char* objfile, char* mtlfile = "") : Object(position, rotation, velocity) 
	{
		model = glmReadOBJ(objfile);
		std::cout << model->numvertices << std::endl;
	}


	virtual Hit intersect(const Ray &ray);
	Vector getUV(Point hit, Vector n);
	Point getHit(double u, double v);
};

