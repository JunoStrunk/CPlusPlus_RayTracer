#include "Material.h"

Material::Material()
{
	isMirror = false;
	color = Vector3f(1.0, 1.0, 1.0);
	ka = Array3f(0.2, 0.2, 0.2);
}

Material::Material(Array3f color)
{
	isMirror = false;
	this->color = color;
	ka = Array3f(0.2, 0.2, 0.2);
}

Material::Material(Array3f color, Array3f ka)
{
	isMirror = false;
	this->color = color;
	this->ka = ka;
}

Material::~Material()
{
}

Array3f Material::evaluate(Vector3f l, Vector3f v, Vector3f n)
{
	return color / M_PI;
}

Array3f Material::getKa()
{
	return ka * color;
}

void Material::setMirror(bool isMirror)
{
	this->isMirror = isMirror;
}

bool Material::getMirror()
{
	return isMirror;
}
