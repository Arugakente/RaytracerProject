//
//  Framework for a raytracer
//  File: object.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef OBJECT_H_AXKLE0OF
#define OBJECT_H_AXKLE0OF

#include "triple.h"
#include "light.h"
#include "rMatrix.h"

class Material;

class Object {
public:
    Material *material;
    Object(Point p,Triple r):position(p),rotation(r){};
    virtual ~Object() { }

    virtual Hit intersect(const Ray &ray) = 0;
    //apply the right translation/rotation to the incomming ray
    Ray transform(const Ray& transformed);
    //apply transformation to vector
    Triple applyTransformation(const Triple& toCorrect);
    //removing transformation form objects (normals for example when transformation have a influence on their calculation)
    Triple removeTransformation(const Triple& toCorrect);
    const Point position;
    const Triple rotation;
};

#endif /* end of include guard: OBJECT_H_AXKLE0OF */
