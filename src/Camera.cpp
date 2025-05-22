#include "Camera.h"

Camera::Camera(Vector3f e, Vector3f worldUp, float eDist, Vector3f w, Vector2f resolution, Vector4f boundaries)
{
	// Assumptions: w is already situated so that it's origin is e

	this->isOrtho = false;
	this->e = e;
	this->w = w;
	this->eDist = eDist;
	if (eDist == 0)
		isOrtho = true;
	else
		isOrtho = false;
	this->resolution = resolution;
	this->boundaries = boundaries;

	// Create Basis
	// Instead of keeping track of u and v, at first (?) keep track of view direction
	// Cross product view direction w = (x,y,z), with it's orthogonal vector on the same plane (-y, x, 0)
	// This will give us the upward direction, v, the cross product of those two will be u.
	v = (w.cross(worldUp));
	u = v.cross(w);
}

Camera::~Camera()
{
}

Ray Camera::CastRay(int i, int j)
{
	float uScalar = boundaries.x() + (boundaries.y() - boundaries.x()) * ((i + 0.5) / resolution.x());
	float vScalar = boundaries.w() + (boundaries.z() - boundaries.w()) * ((j + 0.5) / resolution.y());

	Vector3f origin;
	Vector3f direction;

	if (isOrtho)
	{
		origin = e + (u * uScalar) + (v * vScalar);
		direction = -w;
	}
	else
	{
		origin = e;
		direction = (-eDist * w) + (u * uScalar) + (v * vScalar);
	}

	Ray ray(origin, direction);

	return ray;
}