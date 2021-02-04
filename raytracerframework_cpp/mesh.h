#pragma once

#include "object.h"
#include "triangle.h"
#include "glm.h"
#include <vector>

class Mesh : public Object
{
public:
	GLMmodel* model;
	std::vector<Triangle> triangles;
	float scale = 1.0;

	Mesh(Point position, Triple rotation, Triple velocity, char* objfile, float scale = 1.0) : Object(position, rotation, velocity), scale(scale)
	{
		model = glmReadOBJ(objfile);
		std::cout << model->numvertices << std::endl;

		GLMgroup* group = model->groups;
		while (group) {
			for (int k = 0; k < group->numtriangles; k++) //for each triangle
			{
				GLMtriangle* tr = model->triangles + group->triangles[k];

				float* v1 = model->vertices + 3 * tr->vindices[0];
				float* v2 = model->vertices + 3 * tr->vindices[1];
				float* v3 = model->vertices + 3 * tr->vindices[2];

				Point p1 = scale * Point(v1[0], v1[1], v1[2]);
				Point p2 = scale * Point(v2[0], v2[1], v2[2]);
				Point p3 = scale * Point(v3[0], v3[1], v3[2]); //position relative to object

				Point bary = (p1 + p2 + p3) / 3;
				Point offset1 = p1 - bary;
				Point offset2 = p2 - bary;
				Point offset3 = p3 - bary;
				                                   //we don't double rotation and velocity
				Triangle t = Triangle(position + bary, Vector(0, 0, 0), Vector(0, 0, 0), offset1, offset2, offset3);
				triangles.push_back(t);
			}
			group = group->next;
		}
	}


	virtual Hit intersect(const Ray &ray);
	Vector getUV(Point hit, Vector n);
	Point getHit(double u, double v);
};

