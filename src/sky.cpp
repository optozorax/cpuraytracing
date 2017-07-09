#include "render/ray.h"
#include "render/vector3.h"
#include "render/intersection.h"
#include "render/renderer.h"

#include "objects/sky.h"

namespace gui {
	bool SkyMaterial::scatter(
		const Ray& in,
		const Intersection& intersection,
		Vector3& attenuation,
		Ray& scattered,
		bool& light) const {

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

		intersection.t = tMax-0.001f;
		intersection.position = ray.origin + ray.direction * intersection.t;
		intersection.normal = Vector3(0);
		intersection.material = material;

		return true;
	}
}