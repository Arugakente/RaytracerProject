#include "rMatrix.h"

Vector rMatrix::rotate(Vector rotated)
{
    Vector toReturn;
    toReturn.x = rotated.x*mat[0][0] + rotated.y*mat[1][0] + rotated.z*mat[2][0];
    toReturn.y = rotated.x*mat[0][1] + rotated.y*mat[1][1] + rotated.z*mat[2][1];
    toReturn.z = rotated.x*mat[0][2] + rotated.y*mat[1][2] + rotated.z*mat[2][2];
    return toReturn;
}