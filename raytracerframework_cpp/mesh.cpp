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

	std::cout << r << std::endl << std::endl;
	return res;
}

std::pair<Triangle, GLMtriangle*>* Mesh::getTriangle(Point hit)
{
	int i = 0;
	for (auto t_pair : triangles) {

		//hit = applyTransformation(hit);

		Triangle t = t_pair.first;
		Vector N = (t.v2Offset - t.v1Offset).cross(t.v3Offset - t.v1Offset);
		N.normalize();

		std::cout << i << " : " << abs((hit - t.position).dot(N)) << std::endl;

		
		//if (abs((hit - t.position).dot(N)) > 0.1) continue;

		//std::cout << "hello" << std::endl;

		Vector e1 = t.v2Offset - t.v1Offset;
		Vector e2 = t.v3Offset - t.v2Offset;
		Vector e3 = t.v1Offset - t.v3Offset;

		Vector C0 = hit - t.v1Offset;
		Vector C1 = hit - t.v2Offset;
		Vector C2 = hit - t.v3Offset;

		if (N.dot(e1.cross(C0)) > 0 &&
			N.dot(e2.cross(C1)) > 0 &&
			N.dot(e3.cross(C2)) > 0) {
			std::cout << i << std::endl;
			return &t_pair;
		}
		i++;
	}
	return nullptr;
}


Vector Mesh::getUV(Point hit, Vector n)
{
	std::cout << hit << std::endl;

	if (model->numtexcoords == 0) return Vector(0, 0, 0); //has no texture coordinates

	std::pair<Triangle, GLMtriangle*>* t_pair = getTriangle(hit); //we get the corresponding triangle
	if (t_pair == nullptr) std::cerr << "This point is not in a triangle" << std::endl;

	Triangle t = t_pair->first;
	GLMtriangle* tr = t_pair->second;

	//texcoords Interpolation, taken from codeplea.com/triangular-interpolation

	double w1 = (t.v2Offset.y - t.v3Offset.y) * (hit.x - t.v3Offset.x) + (t.v3Offset.x - t.v2Offset.x) * (hit.y - t.v3Offset.y) /
		( (t.v2Offset.y - t.v3Offset.y) * (t.v1Offset.x - t.v3Offset.x) + (t.v3Offset.x - t.v2Offset.x) * (t.v1Offset.y - t.v3Offset.y) );
	double w2 = (t.v3Offset.y - t.v1Offset.y) * (hit.x - t.v3Offset.x) + (t.v1Offset.x - t.v3Offset.x) * (hit.y - t.v3Offset.y) /
		((t.v2Offset.y - t.v3Offset.y) * (t.v1Offset.x - t.v3Offset.x) + (t.v3Offset.x - t.v2Offset.x) * (t.v1Offset.y - t.v3Offset.y));
	double w3 = 1 - w1 - w2;

	double u = w1 * (model->texcoords + 3 * tr->tindices[0])[0] + w2 * (model->texcoords + 3 * tr->tindices[1])[0] + w3 * (model->texcoords + 3 * tr->tindices[2])[0];
	double v = w1 * (model->texcoords + 3 * tr->tindices[0])[1] + w2 * (model->texcoords + 3 * tr->tindices[1])[1] + w3 * (model->texcoords + 3 * tr->tindices[2])[1];

	return Vector(u, v);
}


Point Mesh::getHit(double u, double v)
{
	return Point(0, 0, 0);
}