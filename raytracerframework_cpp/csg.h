#pragma once

#include "object.h"
#include <tuple>
#include <vector>

enum CombinationMode_t{Difference,Union,Intersection};

class Csg : public Object
{
private:
    std::pair<Hit,int> getNearestIntersectedObj(const Ray& ray);
public:
	Csg(Point position, Triple rotation, Triple velocity) : Object(position, rotation, velocity) {}

    int intersectCount = 0;
    
	virtual Hit intersect(const Ray &ray);
    void addElement(Object* newObj,CombinationMode_t cMode);

    std::vector<std::pair<Object*,CombinationMode_t>> objects;

	Point getHit(double u, double v);
};
