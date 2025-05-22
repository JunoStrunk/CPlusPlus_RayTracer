#include "Triangle.h"

Triangle::Triangle(Vector3f a, Vector3f b, Vector3f c, Material *m)
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->m = m;
}

HitRecord Triangle::hit(Ray r, float t0, float t1)
{
	Matrix3f A{
		{a.x() - b.x(), a.x() - c.x(), r.getDir().x()},
		{a.y() - b.y(), a.y() - c.y(), r.getDir().y()},
		{a.z() - b.z(), a.z() - c.z(), r.getDir().z()}};

	Vector3f B(a.x() - r.getOrg().x(), a.y() - r.getOrg().y(), a.z() - r.getOrg().z());

	float eihf = A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1);
	float gfdi = A(0, 2) * A(2, 1) - A(0, 1) * A(2, 2);
	float dheg = A(0, 1) * A(1, 2) - A(1, 1) * A(0, 2);
	float akjb = A(0, 0) * B.y() - B.x() * A(1, 0);
	float jcal = B.x() * A(2, 0) - A(0, 0) * B.z();
	float blkc = A(1, 0) * B.z() - B.y() * A(2, 0);
	float M = A(0, 0) * eihf + A(1, 0) * gfdi + A(2, 0) * dheg;

	float t = -((A(2, 1) * akjb + A(1, 1) * jcal + A(0, 1) * blkc) / M);

	if (t < t0 || t > t1)
		return HitRecord();

	float gamma = (A(2, 2) * akjb + A(1, 2) * jcal + A(0, 2) * blkc) / M;

	if (gamma < 0 || gamma > 1)
		return HitRecord();

	float beta = (B.x() * eihf + B.y() * gfdi + B.z() * dheg) / M;

	if (beta < 0 || beta > (1 - gamma))
		return HitRecord();

	Vector3f normal = ((b - a).cross(c - a)).normalized();
	return HitRecord(t, normal, m);
}