#include"csg.h"

std::pair<Hit,size_t> Csg::getNearestIntersectedObj(const Ray& ray)
{
	Hit min_hit(std::numeric_limits<long double>::infinity(), Vector());
	size_t foundIdx = -1;
	for (size_t i = 0; i < objects.size(); ++i) {
		Hit hit(objects.at(i).first->intersect(ray));
		if (hit.t < min_hit.t) {
			min_hit = hit;
			foundIdx = i;
		}
	}
	return(std::make_pair(min_hit,foundIdx));
}

Hit Csg::intersect(const Ray &ray)
{
    Ray transformedRay = transform(ray);

    auto contact = getNearestIntersectedObj(transformedRay);

    if(contact.second != -1)
    {
        if(objects.at(contact.second).second == Union)
        {
			contact.first.N = removeTransformation(contact.first.N);
            return contact.first;
        }
    }
	return Hit::NO_HIT();
}

Point Csg::getHit(double u, double v)
{
	return Point(0, 0, 0);
}

void Csg::addElement(Object* newObj,CombinationMode_t cMode)
{
    objects.push_back(std::pair<Object*,CombinationMode_t>(newObj,cMode));
}