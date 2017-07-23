#include <iostream>

#include "render/math.h"
#include "render/film.h"
#include "render/camera.h"
#include "render/renderer.h"

#include "objects/shape.h"
#include "render/intersection.h"
#include "render/ray.h"

#include "render/time.h"

#include "objects/all.h"
#include "materials/all.h"

#include "interval.h"

using namespace gui;

void initScene(Scene& scene) {
	auto& shapes = scene.shapes;
	shapes.push_back(new Sky());
	for (int i = -5; i < 5; ++i) {
		for (int j = -5; j < 5; ++j) {
			shapes.push_back(new Sphere(Vector3(i, 0.3, j), 0.3, new Diffuse(Vector3(0.9f, abs(i+j)/10.0f, 0.9f))));
		}
	}
	shapes.push_back(new Sphere(Vector3(0, -1000, 0), 1000, new Diffuse(Vector3(0.8f))));
}

int main() {
	const float r = 10;
	const float h = 10;

	float pastTime = Time::getCurrentTime();
	int count = 0;
	#pragma omp parallel for num_threads(2)
	for (int j = 0; j < 90; ++j) {
		int i = 240;
		Scene scene;
		initScene(scene);
		scene.shapes.push_back(
			new Implicit<Torus>(new Metal(Vector3(0.9f), 0), 
			new TrRotate(Vector3(j*2, 0, 0)/180.0f * Math::PI, 
				new TrMove(Vector3(-1.5, 2, 0), nullptr)
			)
		, new float[2]{1.0f, 1.0f/3.0f}));
		scene.shapes.push_back(
			new Implicit<Torus>(new Dielectric(1.5f), 
			new TrRotate(Vector3(j*2, 0, 0)/180.0f * Math::PI, 
				new TrMove(Vector3(1.5, 2, 0), nullptr)
			)
		, new float[2]{1.0f, 1.0f/3.0f}));

		scene.shapes.push_back(
			new Implicit<Torus>(new Diffuse(Vector3(0.9f, 0.3f, 0.9f)), 
			new TrScale(Vector3(0.5f),
				new TrRotate(Vector3(0, j*4, 0)/180.0f * Math::PI,
					new TrMove(Vector3(0, 2, 1.5), nullptr)
				)
			)
		, new float[2]{1.0f, 1.0f/5.0f}));

		float alpha = i/180.0 * Math::PI;

		Film film(500, 250);

		Camera camera(film, 15, 0, 12);
		camera.position = Vector3(cos(alpha)*r, fabs(sin(alpha)*h)+1, sin(alpha)*r);
		camera.lookAt(Vector3(0, 2, 0));

		Renderer renderer(400);
		renderer.log = false;
		renderer.render(scene, camera);

		film.gammaCorrection();

		char s[100] = {};
		sprintf(s,"imgs/imageR%3d.bmp", j);
		film.writeBMP(s);

		#pragma omp critical (section1)
		{
			count++;
			std::cout << "\rCount: " << count;
			Time::writeTimeStatus(pastTime, count/90.0f);
		}
	}
	Time::writeTotalTime(pastTime);

	return 0;
}