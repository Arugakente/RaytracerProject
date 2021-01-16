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
	double xStretch;
	uint32_t viewWidth;
	uint32_t viewHeight;
	double exposureTime;

	Camera(Triple e, Triple c, Triple u, double xs, int w, int h, double eT) : eye(e), center(c), up(u), xStretch(xs), viewWidth(w), viewHeight(h), exposureTime(eT)
	{
		Vector front = (eye - center).normalized();
		side = up.cross(front).normalized();
	}

	//fonction to return the Ray pointing at a pixel
	Ray rayAt(Point pix);

	~Camera();
};

