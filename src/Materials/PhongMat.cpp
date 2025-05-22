#include "PhongMat.h"

PhongMat::PhongMat()
{
	color = Vector3f(1.0, 1.0, 1.0);
	ka = Array3f(0.2, 0.2, 0.2);
	ks = Array3f(0.2, 0.2, 0.2);
}

PhongMat::PhongMat(Array3f color)
{
	this->color = color;
	ka = Array3f(0.2, 0.2, 0.2);
	ks = Array3f(0.2, 0.2, 0.2);
	p = 100;
}

PhongMat::PhongMat(Array3f color, Array3f ka)
{
	this->color = color;
	this->ka = ka;
	ks = Array3f(0.2, 0.2, 0.2);
	p = 100;
}

PhongMat::PhongMat(Array3f color, Array3f ka, Array3f ks)
{
	this->color = color;
	this->ka = ka;
	this->ks = ks;
	p = 100;
}

PhongMat::PhongMat(Array3f color, Array3f ka, Array3f ks, float p)
{
	this->color = color;
	this->ka = ka;
	this->ks = ks;
	this->p = p;
}

PhongMat::~PhongMat()
{
}

Array3f PhongMat::evaluate(Vector3f l, Vector3f v, Vector3f n)
{
	Vector3f h = (l + v).normalized();
	return ((color / M_PI) + ks * pow(std::max((float)0, n.dot(h)), (float)p));
}

Array3f PhongMat::getKa()
{
	return ka;
}