//
//  Framework for a raytracer
//  File: torus.cpp
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

#include "torus.h"
#include "deg4solver.h"
#include <iostream>
#include <math.h>

/************************** torus **********************************/

//relying on computation system described in http://blog.marcinchwedczuk.pl/ray-tracing-torus
//using solver from the "graphics gems" book suggested on the website @ https://github.com/erich666/GraphicsGems/blob/master/gems/Roots3And4.c

Hit Torus::intersect(const Ray &ray)
{
    //transforming ray to to match actual torus position.
	Ray TransformedRay = transform(ray);

	double components[5];	// coefficient array for the quartic equation
	double solutions[4];

    //fourth order equation solving
	double dotProdRayDir = TransformedRay.D.dot(TransformedRay.D);
    double dotOriginMinRadiuses = TransformedRay.O.dot(TransformedRay.O)-(r*r+R*R);
	double originDotDirection = TransformedRay.O.dot(TransformedRay.D);
	double Rsquaredmultiplied = 4.0*R*R;

    components[0] = dotOriginMinRadiuses*dotOriginMinRadiuses-Rsquaredmultiplied*(r*r-TransformedRay.O.y*TransformedRay.O.y);
    components[1] = 4.0*dotOriginMinRadiuses*originDotDirection+2.0*Rsquaredmultiplied*TransformedRay.O.y*TransformedRay.D.y;
    components[2] = 2.0*dotProdRayDir*dotOriginMinRadiuses+4.0*originDotDirection*originDotDirection+Rsquaredmultiplied*TransformedRay.D.y*TransformedRay.D.y;
    components[3] = 4.0*dotProdRayDir*originDotDirection;
    components[4] = dotProdRayDir*dotProdRayDir;

    int rootCount = SolveQuartic(components,solutions);

    double t = numeric_limits<double>::max();

    bool intersected = false;

    if(rootCount != 0)
        for(int i=0;i<rootCount;i++)
            if(solutions[i] > 1e-9)
            {
                intersected = true;
                if(solutions[i] < t)
                    t = solutions[i];
            }

    if(!intersected)
    {
        return Hit::NO_HIT();
    }

    Point contact = (TransformedRay.O+(TransformedRay.D*t));
    double squaredRadiuses = r*r+R*R;
    double dotContact = contact.dot(contact);

    Vector multiplier(dotContact-squaredRadiuses,dotContact-squaredRadiuses+2*R*R,dotContact-squaredRadiuses);

    Vector N = 4.0*contact*multiplier;
    N = removeTransformation(N);
    N.normalize();

    return Hit(t,N);
}
