#include "Ray.h"

Ray::Ray(Vector3f origin, Vector3f direction)
{
	this->origin = origin;
	this->direction = direction;
}

Ray::~Ray()
{
}

Vector3f Ray::getDir()
{
	return direction;
}

Vector3f Ray::getOrg()
{
	return origin;
}

void Ray::mirrorOffset()
{
	origin += direction * 0.01;
}

Vector3f Ray::evaluate(float t)
{
	return origin + t * direction;
}