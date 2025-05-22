#include <iostream>
#include "Surface.h"
#include "Light.h"
#include <vector>
#pragma once

class Scene : public Surface
{
private:
	std::vector<Surface *> surfaces;
	std::vector<Light *> lights;

public:
	Scene();
	~Scene();
	HitRecord hit(Ray r, float t0, float t1);

	void addSurface(Surface *s);
	void addLight(Light *l);
	std::vector<Surface *> getSurfaces();
	std::vector<Light *> getLights();
};
