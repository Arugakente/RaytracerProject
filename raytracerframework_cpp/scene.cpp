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
#include <algorithm>


std::pair<Hit,Object*> Scene::getNearestIntersectedObj(const Ray& ray)
{
	Hit min_hit(std::numeric_limits<long double>::infinity(), Vector());
	Object *obj = NULL;
	for (unsigned int i = 0; i < objects.size(); ++i) {
		Hit hit(objects[i]->intersect(ray));
		if (hit.t < min_hit.t) {
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

	Material *material = nearest.second->material;  //the hit objects material
	Point hit = ray.at(nearest.first.t);            //the hit point
	Vector N = nearest.first.N;                     //the normal at hit point
	Vector V = -ray.D;                              //the view vector


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
	*        long double*Color        scales each color component (r,g,b)
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

		Color refractionColor = Color(0.0, 0.0, 0.0);
		if (material->refract && currentReflexion < maxRecursionDepth) 
		{
			Vector Vnorm = ray.D;
			//Vnorm.normalize();
			Vector Nnorm = N;

			long double cosRefract = Vnorm.dot(Nnorm);
			if (cosRefract >  1.0) cosRefract =  1.0;
			if (cosRefract < -1.0) cosRefract = -1.0;

			long double etaIn = 1.0;
			long double etaOut = material->eta;

			if (cosRefract < 0.0) cosRefract = -cosRefract;  //we are getting in the object
			else {
				std::swap(etaIn, etaOut);  //we are getting out of the object
				Nnorm = -Nnorm;
			}

			long double eta = etaIn / etaOut;
			long double k = 1 - eta * eta * (1 - cosRefract * cosRefract);

			Vector RefractionRay = Vector(0.0, 0.0, 0.0);
			if (k >= 0) {
				RefractionRay = eta * Vnorm + (eta * cosRefract - sqrt(k)) * Nnorm;
				RefractionRay.normalize();
				refractionColor = trace(Ray(hit+RefractionRay*0.01, RefractionRay), minRange, maxRange, currentReflexion+1);
			}
		}

		for (auto light : lights)
		{
			
			Vector L = light->position - hit;
			L.normalize();

			Vector R = 2 * (L.dot(N))*N - L;
			R.normalize();

			//computation of intersection with othe objects(reflexion) and in between lights(shadows)
			auto obstacle = getNearestIntersectedObj(Ray(hit+L*0.01, L));
			long double t = obstacle.first.t;

			if (!shadows || !obstacle.second || t > (light->position - hit).length()) {
				long double cosineDiff = L.dot(N);
				long double cosineSpec = R.dot(V);
				if (cosineDiff >= 0.0) Id += light->color * material->kd * cosineDiff;
				if(cosineSpec >= 0.0)
					Is += light->color * material->ks * pow(cosineSpec, material->n);
			}
		}

		//computation of reflexion and refraction color:
		Color reflexionColor = Color(0.0, 0.0, 0.0);

		if (material->ks != 0.0 && currentReflexion < maxRecursionDepth && !material->refract)
		{
			Vector ReflexionRay = 2*(V.dot(N))*N-V;
			ReflexionRay.normalize();
			reflexionColor = trace(Ray(hit+ReflexionRay*0.01,ReflexionRay),minRange,maxRange,currentReflexion+1);
		}

		Is += reflexionColor * material->ks;

		if(material->refract)
			color = refractionColor + Is;
		else
			color = (Ia + Id) * material->color + Is;
	}

	// zBuffer ------
	else if (renderMode == zBuffer || renderMode == zBufferAuto)
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

	float nearPoint = std::numeric_limits<float>::min();
	float farPoint = std::numeric_limits<float>::max();

	if (renderMode == zBuffer) {
		nearPoint = nearPlane;
		farPoint = farPlane;
	}

	if(renderMode == zBufferAuto)
    {
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
            {
                Point pixel(x+0.5, h-1-y+0.5, 0);
				float currentPoint;

				if (hasCamera) currentPoint = getContactZ(camera->rayAt(pixel));
				else currentPoint = getContactZ(Ray(eye, (pixel - eye).normalized()));
					
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

	float offsetX = 0.5/superSamplingFactor;
	float offsetY = 0.5/superSamplingFactor;

	#pragma omp parallel for
    for (int y = 0; y < h; y++) 
	{
        for (int x = 0; x < w; x++) 
		{
			Color sumColor = Color(0.0,0.0,0.0);

			for(int yy = 1;yy<=superSamplingFactor;yy++)
			{
				for(int xx = 1; xx<=superSamplingFactor;xx++)
				{
            		Point pixel(x+(offsetX*xx), h-1-y+(offsetY*yy), 0);
					Ray ray = hasCamera ? camera->rayAt(pixel) : Ray(eye, (pixel - eye).normalized()) ;
            		Color col = trace(ray, farPoint, nearPoint,0);
            		col.clamp();
            		sumColor += col;
				}
			}
			img(x,y) = sumColor/(superSamplingFactor*superSamplingFactor);
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

void Scene::setEye(Triple e) {
	eye = e;
}

void Scene::setShadows(bool b) 
{
	shadows = b;
}

void Scene::setRenderMode(renderMode_t m)
{
	renderMode = m;
}

renderMode_t Scene::getRenderMode()
{
	return renderMode;
}

void Scene::setClippingPlanes(int far, int near)
{
	farPlane = far;
	nearPlane = near;
}

void Scene::setMaxRecursionDepth(int i)
{
	maxRecursionDepth = i;
}

void Scene::setCamera(Camera *c)
{
	camera = c;
}

void Scene::setHasCamera(bool b) 
{
	hasCamera = b;
}

bool Scene::getHasCamera() 
{
	return hasCamera;
}

int Scene::getWidth()
{
	return camera->viewWidth;
}

int Scene::getHeight()
{
	return camera->viewHeight;
}

void Scene::setSuperSamplingFactor(int f)
{
	superSamplingFactor = f;
}

void Scene::setLightSampling(int s)
{
	lightSampling = s;
}
