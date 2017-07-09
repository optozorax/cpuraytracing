#ifndef GUI_DIELECTRIC_H
#define GUI_DIELECTRIC_H

#include "materials/material.h"

namespace gui {
	class Dielectric : public Material {
	public:
		Dielectric() {}

		Dielectric(float ior) : ior(ior)
		{ }

		bool scatter(
			const Ray& in,
			const Intersection& intersection,
			Vector3& attenuation,
			Ray& scattered,
			bool& light) const override;

		float schlick(float cosine, float niOverNt) const;

	public:
		float ior;
	};
}

#endif