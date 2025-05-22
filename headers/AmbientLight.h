#include <iostream>
#include "Light.h"
#pragma once

class AmbientLight : public Light
{
private:
	Vector3f pos;
	Array3f I; // color

public:
	AmbientLight(Vector3f pos, Array3f I);
	~AmbientLight();

	Array3f illuminate(Ray r, HitRecord hrec);
};
