#include <iostream>
#include "Surface.h"
#include "Eigen/Dense"
#pragma once

using Eigen::Vector3f;

class Sphere : public Surface
{
private:
	Vector3f c;	  // center;
	float radius; // radius
	Material *m;  // Material

public:
	Sphere(Vector3f c, float r, Material *m);
	HitRecord hit(Ray r, float t0, float t1);
};
