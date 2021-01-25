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
//  Students:
//    Guillaume Gosset
//    Quentin Lasota
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
#include "constants.h"
#include "material.h"

class Object {
public:
    Material *material;
    Object(Point p, Triple r, Triple v) : initPosition(p), position(p), rotation(r), velocity(v) {};
    virtual ~Object() { }

    virtual Hit intersect(const Ray &ray) = 0;
    //apply the right translation/rotation to the incomming ray
    Ray transform(const Ray& transformed);
    //apply transformation to vector
    Triple applyTransformation(const Triple& toCorrect);
    //removing transformation form objects (normals for example when transformation have a influence on their calculation)
    Triple removeTransformation(const Triple& toCorrect);
	const Point initPosition;
    Point position; //position considering velocity
    const Triple rotation;
	const Triple velocity;

	Color getTexel(Point hit, Vector n);
	virtual Vector getUV(Point hit, Vector n); //we could specify a certain type of uv mapping
    virtual Point getHit(double u, double v) =0; //inverse function of getUV
};

#endif /* end of include guard: OBJECT_H_AXKLE0OF */
