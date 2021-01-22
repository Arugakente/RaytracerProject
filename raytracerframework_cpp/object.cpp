//    
//  Students:
//    Guillaume Gosset
//    Quentin Lasota
//

#include "object.h"

Ray Object::transform(const Ray& transformed)
{
    Vector rotated = transformed.D;
    rotated = rMatrix(rotation.x,rot_x).rotate(rotated);
    rotated = rMatrix(rotation.y,rot_y).rotate(rotated);
    rotated = rMatrix(rotation.z,rot_z).rotate(rotated);

    Point origin = transformed.O-position;
    origin = rMatrix(rotation.x,rot_x).rotate(origin);
    origin = rMatrix(rotation.y,rot_y).rotate(origin);
    origin = rMatrix(rotation.z,rot_z).rotate(origin);
    return Ray(origin,rotated);
}

Triple Object::removeTransformation(const Triple& toCorrect)
{
    Triple corrected = toCorrect;
    corrected = rMatrix(-rotation.z,rot_z).rotate(corrected);
    corrected = rMatrix(-rotation.y,rot_y).rotate(corrected);
    corrected = rMatrix(-rotation.x,rot_x).rotate(corrected);
    return corrected;
}

Triple Object::applyTransformation(const Triple& toCorrect)
{
    Triple corrected = toCorrect;
    corrected = rMatrix(rotation.x,rot_x).rotate(corrected);
    corrected = rMatrix(rotation.y,rot_y).rotate(corrected);
    corrected = rMatrix(rotation.z,rot_z).rotate(corrected);
    return corrected;
}

Color Object::getTexel(Point hit, Vector n)
{
	Vector uv = getUV(hit, n);
	return material->texture->colorAt(uv.x, uv.y);
}


Vector Object::getUV(Point hit, Vector n)
{
	//by default : spherical mapping
	Vector toCenter = (hit - position).normalized();

	return Vector (asinf(toCenter.x)/PI + 0.5, asinf(toCenter.y)/PI + 0.5, 0.0).normalized();
}