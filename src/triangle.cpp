#include "render/ray.h"
#include "render/renderer.h"
#include "render/intersection.h"

#include "objects/triangle.h"

namespace gui {
	float Triangle::area(float a, float b, float c) {
		float p = (a + b + c)/2.0f;
		return sqrtf(p*(p-a)*(p-b)*(p-c));
	}

	bool Triangle::intersect(
		const Ray& ray,
		float tMin,
		float tMax,
		Intersection& intersection) const {

		if (ray.direction.dot(normal)) {
			float t = (-d - ray.origin.dot(normal))/ray.direction.dot(normal);

			if (t > tMin && t < tMax) {
				// Position when ray intersect plane
				Vector3 x = ray.origin + ray.direction * t;

				// Point in triangle <=> area of triangle = sum of inner triangles
				float xx1 = (x1-x).length();
				float xx2 = (x2-x).length();
				float xx3 = (x3-x).length();

				float S1 = area(x1x2, xx1, xx2);
				float S2 = area(x2x3, xx2, xx3);
				float S3 = area(x1x3, xx1, xx3);

				if (fabs((S1 + S2 + S3)/S) < 1.001f) {
					intersection.t = t;
					if (ray.direction.dot(normal) < 0)
						intersection.normal = normal;
					else
						intersection.normal = -normal;
					intersection.position = x + intersection.normal * 0.0001f;
					intersection.material = material;
					return true;
				} else
					return false;
			} else
				// Ray is not in ranges
				return false;
		} else {
			// Ray is parallel plane
			return false;
		}
	}
}
