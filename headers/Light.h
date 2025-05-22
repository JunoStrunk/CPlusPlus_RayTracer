#include <iostream>
#include "Eigen/Dense"
#include "Ray.h"
#include "HitRecord.h"
#pragma once

using Eigen::Array3f;
using Eigen::Vector3f;

class Light
{
private:
	Array3f I; // color

public:
	Light();
	~Light();

	virtual Array3f illuminate(Ray r, HitRecord hrec);
};