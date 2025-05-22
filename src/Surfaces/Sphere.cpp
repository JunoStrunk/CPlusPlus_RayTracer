#include "Sphere.h"

Sphere::Sphere(Vector3f c, float r, Material *m)
{
	this->c = c;
	this->radius = r;
	this->m = m;
}

HitRecord Sphere::hit(Ray r, float t0, float t1)
{
	Vector3f orgDist = r.getOrg() - c;
	float dd = r.getDir().dot(r.getDir());
	float discriminant = pow((r.getDir().dot(orgDist)), 2.0) - (dd * (orgDist.dot(orgDist)) - pow(radius, 2.0));

	if (discriminant < 0)
		return HitRecord();

	float tminus = ((-r.getDir()).dot(orgDist) - sqrt(discriminant)) / dd;
	float tplus = ((-r.getDir()).dot(orgDist) + sqrt(discriminant)) / dd;
	float t;

	if (tminus >= t0 && tminus <= t1)
		t = tminus;
	else if (tplus >= t0 && tplus <= t1)
		t = tplus;
	else
		return HitRecord();

	Vector3f normal = ((r.evaluate(t) - c) / radius).normalized();
	return HitRecord(t, normal, m);
}