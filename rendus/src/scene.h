//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include <tuple>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"

enum renderMode_t{phong, zBuffer , normal};

class Scene
{
private:
	renderMode_t renderMode;
	bool shadows;
	int maxRecursionDepth;
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
public:
    std::pair<Hit,Object*> getNearestIntersectedObj(const Ray& ray);
    Color trace(const Ray &ray,float minRange,float maxRange, int currentReflexion);
        //function used to get the z component associated with the contact point of a ray
        //used to get the far/near limits calculation for z buffer
    float getContactZ(const Ray &ray);
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
	void setShadows(bool b);
	void setRenderMode(renderMode_t m);
    void setEye(Triple e);
	void setMaxRecursionDepth(int i);

    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
