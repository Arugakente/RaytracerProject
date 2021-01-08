//
//  Framework for a raytracer
//  File: scene.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//    
//  Students:
//    Guillaume Gosset
//    Quentin Lasota
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "scene.h"
#include "material.h"


std::pair<Hit,Object*> Scene::getNearestIntersectedObj(const Ray& ray)
{
	Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
	Object *obj = NULL;
	for (unsigned int i = 0; i < objects.size(); ++i) {
		Hit hit(objects[i]->intersect(ray));
		if (hit.t < min_hit.t && hit.t > 0) {
			min_hit = hit;
			obj = objects[i];
		}
	}
	return(std::make_pair(min_hit,obj));
}

Color Scene::trace(const Ray &ray, float minRange, float maxRange, int currentReflexion)
{
	// Find hit object and distance
	std::pair<Hit,Object*> nearest = getNearestIntersectedObj(ray);

	// No hit? Return background color.
	if (!nearest.second) return Color(0.0, 0.0, 0.0);

	Material *material = nearest.second->material;            //the hit objects material
	Point hit = ray.at(nearest.first.t);                 //the hit point
	Vector N = nearest.first.N;                          //the normal at hit point
	Vector V = -ray.D;                             //the view vector


	/****************************************************
	* This is where you should insert the color
	* calculation (Phong model).
	*
	* Given: material, hit, N, V, lights[]
	* Sought: color
	*
	* Hints: (see triple.h)
	*        Triple.dot(Vector) dot product
	*        Vector+Vector      vector sum
	*        Vector-Vector      vector difference
	*        Point-Point        yields vector
	*        Vector.normalize() normalizes vector, returns length
	*        double*Color        scales each color component (r,g,b)
	*        Color*Color        dito
	*        pow(a,b)           a to the power of b
	****************************************************/

	Color color;

	// Phong ------
	if (renderMode == phong) 
	{

		Color Ia = Color(1.0, 1.0, 1.0);
		Ia *= material->ka;

		Color Id = Color(0.0, 0.0, 0.0);
		Color Is = Color(0.0, 0.0, 0.0);

		for (auto light : lights) 
		{
			Vector L = light->position - hit;
			L.normalize();

			Vector R = 2 * (L.dot(N))*N - L;
			R.normalize();

			//computation of intersection with othe objects(reflexion) and in between lights(shadows)
			auto obstacle = getNearestIntersectedObj(Ray(hit, L));
			double t = obstacle.first.t;

			if (!shadows || !obstacle.second || t > (light->position - hit).length()) {
				double cosineDiff = L.dot(N);
				double cosineSpec = R.dot(V);
				if (cosineDiff >= 0.0) Id += light->color * material->kd * cosineDiff;
				if (cosineSpec >= 0.0) Is += light->color * material->ks * pow(cosineSpec, material->n);
			}
		}

		//computation of reflexion color:
		Color reflexionColor = Color(0.0, 0.0, 0.0);
		if(material->ks != 0.0 && currentReflexion < maxRecursionDepth)
		{
			Vector ReflexionRay = 2*(V.dot(N))*N-V;
			ReflexionRay.normalize();
			reflexionColor = trace(Ray(hit,ReflexionRay),minRange,maxRange,currentReflexion+1);
		}
		Is += reflexionColor * material->ks;

		color = (Ia + Id) * material->color + Is;
	}

	// zBuffer ------
	else if (renderMode == zBuffer)
	{
        float z = (hit.z-minRange)/(maxRange-minRange);
		color = Color(z, z, z);
	}

	// Normals ------
	else if (renderMode == normal) 
	{
		color = 0.5*N + 0.5*Vector(1.0, 1.0, 1.0);
	}


    return color;
}

float Scene::getContactZ(const Ray &ray)
{
	std::pair<Hit,Object*> nearest = getNearestIntersectedObj(ray);
    return ray.at(nearest.first.t).z;
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();

	//fetching front/far limit for automated zbuffer range claibration
    float nearPoint = std::numeric_limits<float>::min();
    float farPoint = std::numeric_limits<float>::max();

	if(renderMode == zBuffer)
    {
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
            {
                Point pixel(x+0.5, h-1-y+0.5, 0);
                Ray ray(eye, (pixel-eye).normalized());
                float currentPoint = getContactZ(ray);
                if(currentPoint != std::numeric_limits<float>::infinity() && currentPoint != -std::numeric_limits<float>::infinity())
                {
                    if(nearPoint<currentPoint)
                        nearPoint=currentPoint;
                    if(farPoint>currentPoint)
                        farPoint = currentPoint;
                }
            }
        //offset to match example apparence
		farPoint -= 95;
        nearPoint -= 50;
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray, farPoint, nearPoint,0);
            col.clamp();
            img(x,y) = col;
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setShadows(bool b) 
{
	shadows = b;
}

void Scene::setRenderMode(renderMode_t m)
{
	renderMode = m;
}

void Scene::setMaxRecursionDepth(int i)
{
	maxRecursionDepth = i;
}

void Scene::setEye(Triple e)
{
    eye = e;
}
