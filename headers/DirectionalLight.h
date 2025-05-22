#include <iostream>
#include "Light.h"
#pragma once

class DirectionalLight : public Light
{
private:
	Vector3f pos;
	Array3f I;
	Array3f H;

public:
	DirectionalLight(Vector3f pos, Array3f I, Array3f H);
	~DirectionalLight();

	Array3f illuminate(Ray ray, HitRecord hrec);
};
