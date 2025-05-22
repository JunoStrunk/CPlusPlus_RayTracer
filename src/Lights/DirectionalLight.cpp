#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Vector3f pos, Array3f I, Array3f H)
{
	this->pos = pos;
	this->I = I;
	this->H = H;
}

DirectionalLight::~DirectionalLight()
{
}

Array3f DirectionalLight::illuminate(Ray ray, HitRecord hrec)
{
	if (hrec.getT() < 0)
		return Array3f(0.0, 0.0, 0.0);
	Vector3f x = ray.evaluate(hrec.getT());
	Vector3f l = (pos - x).normalized();
	Vector3f normal = hrec.getNormal();
	Array3f E = (H * std::max((float)0, normal.dot(l)) * I);
	Array3f k = hrec.getMat()->evaluate(l, (-ray.getDir() / ray.getDir().norm()), normal);
	return k * E;
}