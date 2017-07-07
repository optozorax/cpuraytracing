#include "render/ray.h"
#include "render/math.h"
#include "render/intersection.h"

#include "materials/diffuse.h"

namespace gui {
	bool Diffuse::scatter(
		const Ray& in,
		const Intersection& intersection,
		Vector3& attenuation,
		Ray& scattered) const {

		attenuation = albedo;
		scattered.origin = intersection.position + intersection.normal * 0.001f;
		scattered.direction = intersection.normal + Math::randomSphere();
		return true;
	}
}
