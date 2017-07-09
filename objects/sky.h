#ifndef GUI_SKY_H
#define GUI_SKY_H

#include "objects/shape.h"
#include "render/vector3.h"

namespace gui {
	class SkyMaterial : public Material {
	public:
		SkyMaterial(Vector3 downColor, Vector3 upColor) : 
			downColor(downColor), upColor(upColor)
		{ }

		bool scatter(
			const Ray& in,
			const Intersection& intersection,
			Vector3& attenuation,
			Ray& scattered,
			bool& light
		) const override;

	private:
		Vector3 downColor, upColor;
	};

	class Sky : public Shape {
	public:
		Sky(Vector3 downColor = Vector3(1), 
			Vector3 upColor = Vector3(0.5f, 0.7f, 1))
			: Shape(new SkyMaterial(downColor, upColor))
		{ }

		bool intersect(
			const Ray& ray,
			float tMin,
			float tMax,
			Intersection& intersection
		) const override;
	};
}

#endif
