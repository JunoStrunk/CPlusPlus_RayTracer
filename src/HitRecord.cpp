#include "HitRecord.h"

HitRecord::HitRecord()
{
	this->t = -1;
	this->n = Vector3f(0.0, 0.0, 0.0);
	this->m = nullptr;
}

HitRecord::HitRecord(float t, Vector3f n, Material *m)
{
	this->t = t;
	this->n = n;
	this->m = m;
}

HitRecord::~HitRecord()
{
}

float HitRecord::getT()
{
	return t;
}

Vector3f HitRecord::getNormal()
{
	return n;
}

Material *HitRecord::getMat()
{
	return m;
}
