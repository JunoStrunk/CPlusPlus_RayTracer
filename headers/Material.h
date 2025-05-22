#include <iostream>
#include <cmath>
#include "Eigen/Dense"
#pragma once

using Eigen::Array3f;
using Eigen::Vector3f;

class Material
{
private:
	bool isMirror;
	Array3f color;
	Array3f ka; // ambient coefficient

public:
	Material();
	Material(Array3f color);
	Material(Array3f color, Array3f ka);
	~Material();

	virtual Array3f evaluate(Vector3f l, Vector3f v, Vector3f n);
	Array3f getKa();
	void setMirror(bool isMirror);
	bool getMirror();
};
