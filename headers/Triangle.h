#include <iostream>
#include "Eigen/Dense"
#include "Surface.h"
#pragma once

using Eigen::Matrix3f;
using Eigen::Vector3f;

class Triangle : public Surface
{
private:
	Vector3f a;
	Vector3f b;
	Vector3f c;
	Material *m;

public:
	Triangle(Vector3f a, Vector3f b, Vector3f c, Material *m);
	HitRecord hit(Ray r, float t0, float t1);
};
