#include "PointLight.h"

PointLight::PointLight(Vector3f pos, Array3f I)
{
	this->pos = pos;
	this->I = I;
}

PointLight::~PointLight()
{
}

Array3f PointLight::illuminate(Ray ray, HitRecord hrec)
{
	if (hrec.getT() < 0)
		return Array3f(0.0, 0.0, 0.0);
	Vector3f x = ray.evaluate(hrec.getT());
	float r = (pos - x).norm();
	Vector3f l = (pos - x) / r;
	Vector3f normal = hrec.getNormal();
	Array3f E = (std::max((float)0, normal.dot(l)) * I) / pow(r, 2.0);
	Array3f k = hrec.getMat()->evaluate(l, (-ray.getDir() / ray.getDir().norm()), normal);
	return k * E;
}