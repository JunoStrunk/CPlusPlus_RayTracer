#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

HitRecord Scene::hit(Ray r, float t0, float t1)
{
	HitRecord closest;
	for (Surface *obj : surfaces)
	{
		HitRecord hrec = obj->hit(r, t0, t1);
		if (hrec.getT() > 0 && hrec.getT() < std::numeric_limits<float>().max())
		{
			closest = hrec;
			t1 = hrec.getT();
		}
	}
	return closest;
}

void Scene::addSurface(Surface *s)
{
	surfaces.push_back(s);
}

void Scene::addLight(Light *l)
{
	lights.push_back(l);
}

std::vector<Surface *> Scene::getSurfaces()
{
	return surfaces;
}

std::vector<Light *> Scene::getLights()
{
	return lights;
}