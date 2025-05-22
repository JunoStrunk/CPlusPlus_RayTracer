#include <iostream>
#include "Eigen/Dense"
#include "Light.h"
#pragma once

class PointLight : public Light
{
private:
	Vector3f pos;
	Array3f I; // color

public:
	PointLight(Vector3f pos, Array3f I);
	~PointLight();

	Array3f illuminate(Ray r, HitRecord hrec);
};