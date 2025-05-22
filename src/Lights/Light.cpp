#include "Light.h"

Light::Light()
{
	I = Array3f(0.0, 0.0, 0.0);
}

Light::~Light()
{
}

Array3f Light::illuminate(Ray r, HitRecord hrec)
{
	return Array3f(0.0, 0.0, 0.0);
}
