#include "mesh.h"
#include <iostream>
#include <math.h>

Hit Mesh::intersect(const Ray &ray)
{
	Hit res = Hit(std::numeric_limits<double>::infinity(), Vector(0, 0, 0));
	bool hasIntersected = false;
	
	int i = 0;
	int r = -1;
	Triangle* tres = nullptr;

	for (auto t_pair : triangles) {
		Triangle t = t_pair.first;

		Hit h = t.intersect(transform(ray));
		h.N = removeTransformation(h.N);

		if (h.t < res.t)
		{
			if (!hasIntersected) hasIntersected = true;
			res = h;
			r = i;
			tres = &t;
		}
		i++;
	}
	if (!hasIntersected) return Hit::NO_HIT();

	Ray transformedRay = transform(ray);
	return res;
}

std::pair<Triangle, GLMtriangle*>* Mesh::getTriangle(Point hit)
{
	int i = 0;
	for (auto& t_pair : triangles) {

		Triangle t = t_pair.first;
		Vector N = (t.v2Offset - t.v1Offset).cross(t.v3Offset - t.v1Offset);
		N.normalize();

		Ray r = Ray(hit, Vector(0, 0, 0)); //we use a Ray just to transform the hit (to be in a triangle's reference)
		r = transform(r);
		r = t.transform(r);

		Point newHit = r.O;


		if (abs(newHit.dot(N)) < 0.000001) {

			Vector e1 = t.v2Offset - t.v1Offset;
			Vector e2 = t.v3Offset - t.v2Offset;
			Vector e3 = t.v1Offset - t.v3Offset;

			Vector C0 = newHit - t.v1Offset;
			Vector C1 = newHit - t.v2Offset;
			Vector C2 = newHit - t.v3Offset;

			if (N.dot(e1.cross(C0)) > 0 &&
				N.dot(e2.cross(C1)) > 0 &&
				N.dot(e3.cross(C2)) > 0) {
				return &t_pair;
			}
		}
		
		i++;
	}
	return nullptr;
}


Vector Mesh::getUV(Point hit, Vector n)
{
	if (model->numtexcoords == 0) return Vector(0, 0, 0); //has no texture coordinates

	std::pair<Triangle, GLMtriangle*>* t_pair = getTriangle(hit); //we get the corresponding triangle
	if (t_pair == nullptr) std::cerr << "This point is not in a triangle" << std::endl;


	Triangle t = t_pair->first;
	GLMtriangle* tr = t_pair->second;

	Ray r = Ray(hit, Vector(0, 0, 0)); //we use a Ray just to transform the hit (to be in a triangle's reference)
	r = transform(r);
	r = t.transform(r);

	Point newHit = r.O;


	//texcoords Interpolation
	double delta = ((t.v1Offset - t.v2Offset).cross(t.v1Offset - t.v3Offset)).length();

	double w1 = ((newHit - t.v2Offset).cross(newHit - t.v3Offset)).length() / delta;
	double w2 = ((newHit - t.v1Offset).cross(newHit - t.v3Offset)).length() / delta;
	double w3 = ((newHit - t.v1Offset).cross(newHit - t.v2Offset)).length() / delta;

	float* uv1 = model->texcoords + 2 * tr->tindices[0];
	float* uv2 = model->texcoords + 2 * tr->tindices[1];
	float* uv3 = model->texcoords + 2 * tr->tindices[2];
	
	float u = w1 * uv1[0] + w2 * uv2[0] + w3 * uv3[0];
	float v = 1 - (w1 * uv1[1] + w2 * uv2[1] + w3 * uv3[1]); //inverted axis

	return Vector(u, v);
}


Point Mesh::getHit(double u, double v)
{
	return Point(0, 0, 0);
}