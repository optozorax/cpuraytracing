#include <iostream>
#include <omp.h>

#include "render/ray.h"
#include "render/math.h"
#include "render/film.h"
#include "render/camera.h"
#include "render/renderer.h"
#include "render/intersection.h"
#include "render/time.h"

#include "objects/scene.h"

namespace gui {
	Vector3 Renderer::radiance(const Ray& ray, const Scene& scene, int depth) const {
		Intersection intersection;
		Ray scattered;
		Vector3 attenuation;
		bool light = false;

		if (scene.intersect(ray, 0, maxT, intersection)) {
			if (intersection.material->scatter(
				ray, intersection, attenuation, scattered, light) && depth < maxDepth)
				return attenuation * radiance(scattered, scene, ++depth);
			else
				if (light)
					return attenuation;
				else
					return Vector3(0);
		}
		return Vector3(0);
	}

	void Renderer::render(const Scene& scene, Camera& camera) const {
		onRendering();

		int count = 0;
		omp_set_num_threads(threads);
		#pragma omp parallel for shared(camera)
		for(int i = 0; i < camera.film.height; ++i) {
			#pragma omp critical (section2)
			{
			onEveryLine(float(count)/camera.film.height);
			}

			for(int j = 0; j < camera.film.width; ++j) {
				Ray ray = camera.getRay(j, i);
				camera.film.pixels[i][j] += radiance(ray, scene, 0);
				for (int k = 1; k < samples; ++k) {
					Ray ray = camera.getRay(j + Math::random(), i + Math::random());
					camera.film.pixels[i][j] += radiance(ray, scene, 0);
				}
				camera.film.pixels[i][j] /= samples;
			}
			count++;
		}

		onEndRendering();
	}

	void Renderer::onRendering() {
		if (log)
			pastTime = Time::getCurrentTime();
	}

	void Renderer::onEveryLine(float percent) {
		if (log)
			Time::writeTimeStatus(pastTime, percent);
	}

	void Renderer::onEndRendering() {
		if (log)
			Time::writeTotalTime(pastTime);
	}
}
