#ifndef GUI_DIFFUSE_H
#define GUI_DIFFUSE_H

#include "render/vector3.h"
#include "materials/material.h"

namespace gui {
	class Diffuse : public Material {
	public:
		Diffuse(const Vector3& albedo) : albedo(albedo)
		{ }

		bool scatter(
			const Ray& in,
			const Intersection& intersection,
			Vector3& attenuation,
			Ray& scattered,
			bool& light
		) const override;

	public:
		Vector3 albedo;
	};
}

#endif
