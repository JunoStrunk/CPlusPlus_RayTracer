#include <iostream>
#include "Ray.h"
#include "Eigen/Dense"
#pragma once

using Eigen::Vector2f;
using Eigen::Vector3f;
using Eigen::Vector4f;

class Camera
{
private:
	bool isOrtho;
	Vector3f e; // View Point
	Vector3f w; // View Direction
	Vector3f u;
	Vector3f v;
	float eDist;
	Vector2f resolution;
	Vector4f boundaries; //(l,r,t,b)

public:
	Camera(Vector3f e, Vector3f worldUp, float eDist, Vector3f w, Vector2f resolution, Vector4f boundaries);
	~Camera();

	Ray CastRay(int i, int j);
};