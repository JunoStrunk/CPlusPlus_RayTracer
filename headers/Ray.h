#include <iostream>
#include "Eigen/Dense"
#pragma once

using Eigen::Vector3f;

class Ray
{
private:
	Vector3f origin;
	Vector3f direction;

public:
	Ray(Vector3f origin, Vector3f direction);
	~Ray();

	Vector3f getDir();
	Vector3f getOrg();
	void mirrorOffset();

	Vector3f evaluate(float t);
};