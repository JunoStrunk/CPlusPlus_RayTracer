#include "AmbientLight.h"

AmbientLight::AmbientLight(Vector3f pos, Array3f I)
{
	this->pos = pos;
	this->I = I;
}

AmbientLight::~AmbientLight()
{
}

Array3f AmbientLight::illuminate(Ray r, HitRecord hrec)
{
	Array3f ka = hrec.getMat()->getKa();
	return ka * I;
}