#include "render/ray.h"
#include "render/vector3.h"
#include "render/intersection.h"
#include "render/renderer.h"

#include "objects/sky.h"

namespace gui {
	bool SkyMaterial::scatter(
		const Ray& in1,
		const Intersection& intersection,
		Vector3& attenuation,
		Ray& scattered,
		bool& light) const {
		Ray in = in1;

		light = true;

		float k = (in.direction.identity().y + 1) * 0.5f;
		attenuation = downColor * (1 - k) + upColor * k;

		return false;
	}

	bool Sky::intersect(
		const Ray& ray,
		float tMin,
		float tMax,
		Intersection& intersection) const {

		intersection.t = tMax*0.999f;
		intersection.position = ray.origin + ray.direction * intersection.t;
		intersection.normal = -ray.direction;
		intersection.material = material;

		return true;
	}
}