#pragma once
#include<cmath>
#include "triple.h"

#ifndef M_PI
#define M_PI          3.14159265358979323846
#endif

enum rotDirection_t{rot_x, rot_y , rot_z};

class rMatrix
{
    public:
        rMatrix(double angle,rotDirection_t direction)
        {
            for(int i = 0;i<3;i++)
                for(int j = 0;j<3;j++)
                    mat[i][j] = 0.0;
            double radAngle = ((angle*M_PI)/180.0);
            if(direction == rot_x)
            {
                mat[0][0] = 1;
                mat[1][1] = cos(radAngle);
                mat[1][2] = -sin(radAngle);
                mat[2][1] = sin(radAngle);
                mat[2][2] = cos(radAngle);
            }
            if(direction == rot_y)
            {
                mat[0][0] = cos(radAngle);
                mat[0][2] = sin(radAngle);
                mat[1][1] = 1;
                mat[2][0] = -sin(radAngle);
                mat[2][2] = cos(radAngle);
            }
            if(direction == rot_z)
            {
                mat[0][0] = cos(radAngle);
                mat[0][1] = -sin(radAngle);
                mat[1][0] = sin(radAngle);
                mat[1][1] = cos(radAngle);
                mat[2][2] = 1;
            }
        };
        Vector rotate(Vector rotated);
        ~rMatrix(){};

        double mat[3][3];

};
