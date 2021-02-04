#include"csg.h"

std::pair<Hit,int> Csg::getNearestIntersectedObj(const Ray& ray)
{
	Hit min_hit(std::numeric_limits<long double>::infinity(), Vector());
	int foundIdx = -1;
	for (int i = 0; i < objects.size(); ++i) {
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

	int insideCount = 0;
	bool crossed = false;
	bool hitFound = false;
	Hit toReturn = Hit::NO_HIT();
	long double crossedDistance = 0.0;

	while(!hitFound)
	{
		auto contact = getNearestIntersectedObj(transformedRay);
    	if(contact.second != -1)
    	{
        	if(objects.at(contact.second).second == Union)
        	{
				if(insideCount == 0)
				{
					contact.first.N = removeTransformation(contact.first.N);
            		toReturn = contact.first;
					toReturn.t += crossedDistance;
					hitFound = true;
				}
				else
				{
					crossed = !crossed;
					transformedRay.O += transformedRay.D*(contact.first.t+0.01);
					crossedDistance += contact.first.t;
				}
        	}
			else
			{
				if(contact.first.N.dot(transformedRay.D)<=0)
				{
					insideCount += 1;
					transformedRay.O += transformedRay.D*(contact.first.t+0.01);
					crossedDistance += contact.first.t;
				}
				else
				{
					if(insideCount == 1 && crossed)
					{
						contact.first.N = -removeTransformation(contact.first.N);
            			toReturn = contact.first;
						toReturn.t += crossedDistance;

						hitFound = true;
					}
					else
					{
						insideCount -= 1;
						transformedRay.O += transformedRay.D*(contact.first.t+0.01);
						crossedDistance += contact.first.t;
					}
				}
			}
    	}
		else
		{
			hitFound = true;
		}
	}
	return toReturn;
}

Point Csg::getHit(double u, double v)
{
	return Point(0, 0, 0);
}

void Csg::addElement(Object* newObj,CombinationMode_t cMode)
{
    objects.push_back(std::pair<Object*,CombinationMode_t>(newObj,cMode));
}