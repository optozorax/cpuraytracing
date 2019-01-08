#ifndef GUI_TRIANGLE_H
#define GUI_TRIANGLE_H

#include "objects/shape.h"
#include "render/vector3.h"

namespace gui {
	class Triangle : public Shape {
	public:

		float area(float a, float b, float c) const;

		Triangle(const Vector3 x1, Vector3 x2, Vector3 x3, Material* material)
			: Shape(material), x1(x1), x2(x2), x3(x3)
		{ 
			// Equation of plane, every x in plane when: (n, x) + d = 0
			normal = ((x2-x1).cross(x3-x2)).identity();
			d = -normal.dot(x3);
			x1x2 = (x2-x1).length();
			x2x3 = (x3-x2).length();
			x1x3 = (x3-x1).length();
			S = area(x1x2, x2x3, x1x3);
		}

		bool intersect(
			const Ray& ray,
			float tMin,
			float tMax,
			Intersection& intersection
		) const override;

	public:
		Vector3 center;
		Vector3 x1, x2, x3;
		Vector3 normal;
		float d;
		float x1x2, x2x3, x1x3;
		float S;
	};
}

#endif
