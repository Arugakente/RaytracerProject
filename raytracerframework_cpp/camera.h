#pragma once

#include "triple.h"
#include "light.h"

class Camera
{
public:
	Point eye;
	Point center;

	Vector up;
	Vector side;
	Vector front;

	uint32_t viewWidth;
	uint32_t viewHeight;

	int apertureSize;
	int apertureSample;

	Camera(Triple e, Triple c, Triple u, int w, int h,int as,int aSample) : eye(e), center(c), up(u), viewWidth(w), viewHeight(h), apertureSize(as),apertureSample(aSample)
	{
		front = (eye - center).normalized();
		side = up.cross(front).normalized();
	}

	//fonction to return the Ray pointing at a pixel
	Ray rayAt(Point pix);

	~Camera();
};

