#include "mesh.h"
#include <iostream>
#include <math.h>

Hit Mesh::intersect(const Ray &ray)
{
	Hit res = Hit(std::numeric_limits<double>::infinity(), Vector(0, 0, 0));

	bool hasIntersected = false;

	for (size_t k = 0; k < model->numtriangles; k++) //for each triangle
	{
		GLMtriangle* tr = model->triangles + k;

		float* v1 = model->vertices + 3 * tr->vindices[0];
		float* v2 = model->vertices + 3 * tr->vindices[1];
		float* v3 = model->vertices + 3 * tr->vindices[2];
		
		Point p1 = Point(*v1, *(v1 + 1), *(v1 + 2)) /*+ position*/;
		Point p2 = Point(*v2, *(v2 + 1), *(v2 + 2)) /*+ position*/;
		Point p3 = Point(*v3, *(v3 + 1), *(v3 + 2)) /*+ position*/; //real position in scene (not relative to object)

		//std::cout << p1 << " | " << p2 << " | " << p3 << std::endl;

		Ray TransformedRay = ray; //transform(ray);

		Vector CO = -TransformedRay.O; //C - ray.O with C = (0,0,0)
		Vector N = (p2 - p1).cross(p3 - p1);
		N.normalize();

		if (N.dot(TransformedRay.D) == 0)
			continue; //return Hit::NO_HIT();

		long double t = (CO.dot(N) / N.dot(TransformedRay.D));

		if (t < 0) continue; //return Hit::NO_HIT();


		Vector intersect = TransformedRay.O + t * TransformedRay.D;

		//std::cout << intersect << std::endl;

		//check if inside the triangle
		Vector e1 = p2 - p1;
		Vector e2 = p3 - p2;
		Vector e3 = p1 - p3;

		Vector C0 = intersect - p1;
		Vector C1 = intersect - p2;
		Vector C2 = intersect - p3;

		
		if (N.dot(e1.cross(C0)) <= 0 ||
			N.dot(e2.cross(C1)) <= 0 ||
			N.dot(e3.cross(C2)) <= 0)
			continue; //return Hit::NO_HIT();
		
		//N = removeTransformation(N);

		if (t < res.t)
		{
			if (!hasIntersected) hasIntersected = true;
			res = Hit(t, N);
			//std::cout << "ok" << std::endl;
		}
	}

	if (!hasIntersected) return Hit::NO_HIT();
	//std::cout << transform(ray).O + res.t * transform(ray).D << std::endl;

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