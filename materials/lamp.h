#ifndef GUI_LAMP_H
#define GUI_LAMP_H

#include "render/vector3.h"
#include "materials/material.h"

namespace gui {
	class Lamp : public Material {
	public:
		Lamp(const Vector3& albedo) : albedo(albedo)
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
