#include <iostream>
#include "Ray.h"
#include "HitRecord.h"
#include "Material.h"
#pragma once

class Surface
{
private:
public:
	virtual HitRecord hit(Ray r, float t0, float t1);
};