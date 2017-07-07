#include "render/ray.h"
#include "render/vector3.h"
#include "render/intersection.h"

#include "materials/lamp.h"

namespace gui {
	bool Lamp::scatter(
		const Ray& in,
		const Intersection& intersection,
		Vector3& attenuation,
		Ray& scattered,
		bool& light) const {

		light = true;

		attenuation = albedo;

		return false;
	}
}