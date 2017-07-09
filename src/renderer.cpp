#include <windows.h>

#include "render/ray.h"
#include "render/math.h"
#include "render/film.h"
#include "render/camera.h"
#include "render/renderer.h"
#include "render/intersection.h"

#include "objects/scene.h"

namespace gui {
	Vector3 Renderer::radiance(const Ray& ray, const Scene& scene, int depth) const {
		Intersection intersection;
		if (scene.intersect(ray, 0, 10000, intersection)) {
			Ray scattered;
			Vector3 attenuation;
			bool light = false;
			if (intersection.material->scatter(
				ray, intersection, attenuation, scattered, light) && depth < 5) {
				return attenuation * radiance(scattered, scene, ++depth);
			} else {
				if (light)
					return attenuation;
				else
					return Vector3(0);
			}
		} else {
			return Vector3(0);
		}
	}

	void Renderer::render(const Scene& scene, Camera& camera) const {
		onRendering();

		float r = 1.0f / samples;
		for(int i = 0; i < camera.film.height; ++i) {
			onEveryLine(float(i+1)/camera.film.height);
			
			for(int j = 0; j < camera.film.width; ++j) {
				for (int k = 0; k < samples; ++k) {
					Ray ray = camera.getRay(j + Math::random(), i + Math::random());
					camera.film.pixels[i][j] += radiance(ray, scene, 0);
				}
				camera.film.pixels[i][j] *= r;
			}
		}
	}

	float Renderer::getCurrentTime(void) {
		return GetTickCount()/1000.0f;
	}

	void Renderer::onRendering() {
		time = getCurrentTime();
	}

	void Renderer::onEveryLine(float percent) {
		float nowTime = getCurrentTime() - time;
		float approxTime = nowTime/percent;
		float leftTime = approxTime - nowTime;
		printf("\r%5.2f%%; Time: %5.2fs;"
			" Approximate time: %5.2fs; Time left: %5.2fs.", 
			100.0f * percent, nowTime, approxTime, leftTime);
		fflush(stdout);
	}
}
