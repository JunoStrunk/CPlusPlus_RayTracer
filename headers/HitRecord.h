#include <iostream>
#include "Eigen/Dense"
#include "Material.h"
#pragma once

using Eigen::Vector3f;

class HitRecord
{
private:
	float t;	 // Distance to evaluate ray
	Vector3f n;	 // normal
	Material *m; // material

public:
	HitRecord(); // miss
	HitRecord(float t, Vector3f n, Material *m);
	~HitRecord();

	float getT();
	Vector3f getNormal();
	Material *getMat();
};
