//
//  Framework for a raytracer
//  File: triple.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//    
//  Students:
//    Guillaume Gosset
//    Quentin Lasota
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef TRIPLE_H_SEVQHPTA
#define TRIPLE_H_SEVQHPTA

#include <math.h>
#include <iostream>
using namespace std;

class Triple {
public:
    explicit Triple(long double X = 0, long double Y = 0, long double Z = 0)
        : x(X), y(Y), z(Z)
    {
    }

    Triple operator+(const Triple &t) const
    {
        return Triple(x+t.x, y+t.y, z+t.z);
    }

    Triple operator+(long double f) const
    {
        return Triple(x+f, y+f, z+f);
    }

    friend Triple operator+(long double f, const Triple &t)
    {
        return Triple(f+t.x, f+t.y, f+t.z);
    }

    Triple operator-() const
    {
        return Triple( -x, -y, -z);
    }

    Triple operator-(const Triple &t) const
    {
        return Triple(x-t.x, y-t.y, z-t.z);
    }

    Triple operator-(long double f) const
    {
        return Triple(x-f, y-f, z-f);
    }

    friend Triple operator-(long double f, const Triple &t)
    {
        return Triple(f-t.x, f-t.y, f-t.z);
    }

    Triple operator*(const Triple &t) const
    {
        return Triple(x*t.x,y*t.y,z*t.z);
    }

    Triple operator*(long double f) const
    {
        return Triple(x*f, y*f, z*f);
    }

    friend Triple operator*(long double f, const Triple &t)
    {
        return Triple(f*t.x, f*t.y, f*t.z);
    }

    Triple operator/(long double f) const
    {
        long double invf = 1.0/f;
        return Triple(x*invf, y*invf, z*invf);
    }

    Triple& operator+=(const Triple &t)
    {
        x += t.x;
        y += t.y;
        z += t.z;
        return *this;
    }

    Triple& operator+=(long double f)
    {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    Triple& operator-=(const Triple &t)
    {
        x -= t.x;
        y -= t.y;
        z -= t.z;
        return *this;
    }

    Triple& operator-=(long double f)
    {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    Triple& operator*=(const long double f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    Triple& operator/=(const long double f)
    {
        long double invf = 1.0/f;
        x *= invf;
        y *= invf;
        z *= invf;
        return *this;
    }


    long double dot(const Triple &t) const
    {
        return x*t.x + y*t.y + z*t.z;
    }

    Triple cross(const Triple &t) const
    {
        return Triple( y*t.z - z*t.y,
            z*t.x - x*t.z,
            x*t.y - y*t.x);
    }

    long double length() const
    {
        return sqrt(length_2());
    }

    long double length_2() const
    {
        return x*x + y*y + z*z;
    }

    Triple normalized() const
    {
        return (*this) / length();
    }

    void normalize()
    {
        long double l = length();
        long double invl = 1/l;
        x *= invl;
        y *= invl;
        z *= invl;
    }	

    friend istream& operator>>(istream &s, Triple &v);
    friend ostream& operator<<(ostream &s, const Triple &v);

    // Functions for when used as a Color:
    void set(long double f)
    {
        r = g = b = f;
    }

    void set(long double f, long double maxValue)
    {
        set(f/maxValue);
    }

    void set(long double red, long double green, long double blue)
    {
        r = red;
        g = green;
        b = blue;
    }

    void set(long double r, long double g, long double b, long double maxValue)
    {
        set(r/maxValue,g/maxValue,b/maxValue);
    }

    void clamp(long double maxValue = 1.0)
    {
        if (r > maxValue) r = maxValue;
        if (g > maxValue) g = maxValue;
        if (b > maxValue) b = maxValue;
    }

    union {
        long double data[3];
        struct {
            long double x;
            long double y;
            long double z;
        };
        struct {
            long double r;
            long double g;
            long double b;
        };
    };
};

typedef Triple Color;
typedef Triple Point;
typedef Triple Vector;

#endif /* end of include guard: TRIPLE_H_SEVQHPTA */
