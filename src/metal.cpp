#include "render/ray.h"
#include "render/math.h"
#include "render/intersection.h"

#include "materials/metal.h"

namespace gui {
	bool Metal::scatter(
		const Ray& in,
		const Intersection& intersection,
		Vector3& attenuation, 
		Ray& scattered,
		bool& light) const {

		light = false;

		attenuation = albedo;
		scattered.origin = intersection.position + intersection.normal * 0.001f;
		scattered.direction = in.direction.reflect(intersection.normal) + Math::randomSphere() * fuzzy;
		if (scattered.direction.dot(intersection.normal) > 0) return true;
		return false;
	}
}