#include "objects/scene.h"
#include "render/intersection.h"

namespace gui {
	bool Scene::intersect(
		const Ray& ray,
		float tMin,
		float tMax,
		Intersection& intersection) const {
			
		bool intersected = false;
		//for(auto i = shapes.begin(); i != shapes.end(); ++i) {
		for (i : shapes) {
			if(i->intersect(ray, tMin, tMax, intersection)) {
				if(intersection.t < tMax) {
					tMax = intersection.t;
					intersected = true;
				}
			}
		}
		return intersected;
	}
}
