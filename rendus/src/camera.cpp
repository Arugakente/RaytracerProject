#include "camera.h"
#include <cmath>

Ray Camera::rayAt(Point pix) 
{
	Point p = center + side * (1/xStretch) * ((pix.x/(viewWidth/2 - 1)) - 1)*viewWidth/2 + up * ((pix.y / (viewHeight/2 - 1)) - 1)*viewHeight/2;

	Ray ray = Ray(eye, (p - eye).normalized());
	return ray;
}