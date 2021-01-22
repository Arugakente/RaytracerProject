//    
//  Students:
//    Guillaume Gosset
//    Quentin Lasota
//

#include "object.h"

Ray Object::transform(const Ray& transformed)
{
    Vector rotated = transformed.D;
    Point origin = transformed.O-position;

    if(rotation.x != 0.0 || rotation.y != 0.0 || rotation.z != 0.0)
    {
        rotated = rMatrix(rotation.x,rot_x).rotate(rotated);
        rotated = rMatrix(rotation.y,rot_y).rotate(rotated);
        rotated = rMatrix(rotation.z,rot_z).rotate(rotated);


        origin = rMatrix(rotation.x,rot_x).rotate(origin);
        origin = rMatrix(rotation.y,rot_y).rotate(origin);
        origin = rMatrix(rotation.z,rot_z).rotate(origin);
    }

    return Ray(origin,rotated);
}

Triple Object::removeTransformation(const Triple& toCorrect)
{
    //if(rotation.x == 0.0 && rotation.y == 0.0 && rotation.z == 0.0 )
    //    return toCorrect;

    Triple corrected = toCorrect;
    if(rotation.x != 0.0 || rotation.y != 0.0 || rotation.z != 0.0)
    {
        corrected = rMatrix(-rotation.z,rot_z).rotate(corrected);
        corrected = rMatrix(-rotation.y,rot_y).rotate(corrected);
        corrected = rMatrix(-rotation.x,rot_x).rotate(corrected);
    }
    return corrected;
}

Triple Object::applyTransformation(const Triple& toCorrect)
{
    //if(rotation.x == 0.0 && rotation.y == 0.0 && rotation.z == 0.0 )
    //    return toCorrect;

    Triple corrected = toCorrect;
    if(rotation.x != 0.0 || rotation.y != 0.0 || rotation.z != 0.0)
    {
        corrected = rMatrix(rotation.x,rot_x).rotate(corrected);
        corrected = rMatrix(rotation.y,rot_y).rotate(corrected);
        corrected = rMatrix(rotation.z,rot_z).rotate(corrected);
    }
    return corrected;
}