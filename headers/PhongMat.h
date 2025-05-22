#include <iostream>
#include <cmath>
#include "Material.h"
#pragma once

using Eigen::Array3f;
using Eigen::Vector3f;

class PhongMat : public Material
{
private:
	Array3f color;
	Array3f ka;
	Array3f ks;
	float p;

public:
	PhongMat();
	PhongMat(Array3f color);
	PhongMat(Array3f color, Array3f ka);
	PhongMat(Array3f color, Array3f ka, Array3f ks);
	PhongMat(Array3f color, Array3f ka, Array3f ks, float p);
	~PhongMat();

	Array3f evaluate(Vector3f l, Vector3f v, Vector3f n);
	Array3f getKa();
};
