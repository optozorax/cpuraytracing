#include "render/math.h"
#include "render/film.h"
#include "render/camera.h"
#include "render/renderer.h"

#include "objects/all.h"
#include "materials/all.h"

using namespace gui;

void initScene(Scene& scene) {
	auto& shapes = scene.shapes;

	float size = 1000;

	shapes.push_back(new Sphere(Vector3(0, -size, 0), size, new Diffuse(Vector3(0.7f))));
	shapes.push_back(new Sphere(Vector3(0, size+4, 0), size, new Diffuse(Vector3(0.7f))));
	shapes.push_back(new Sphere(Vector3(-size-2, 0, 0), size, new Diffuse(Vector3(0.7f))));
	shapes.push_back(new Sphere(Vector3(0, 0, -size-2), size, new Diffuse(Vector3(0.7f))));
	shapes.push_back(new Sphere(Vector3(0, 0, size+2), size, new Diffuse(Vector3(0.7f))));

	shapes.push_back(new Sphere(
		Vector3(-1, 1, 1), 1, 
		new Metal(Vector3(1, 1, 1))
	));

	shapes.push_back(new Sphere(Vector3(1, 0.2, -1), 0.2, new Diffuse(Vector3(0.7f, 0.7f, 0.7f))));

	shapes.push_back(new Sphere(
		Vector3(0, 4, 0), 1, 
		new Lamp(Vector3(1, 1, 1))
	));
}

void initScene2(Scene& scene) {
	auto& shapes = scene.shapes;
	// FLoor
	shapes.push_back(new Triangle(
		Vector3(-2, 0, -2),
		Vector3(-2, 0, 2),
		Vector3(2, 0, 2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));
	shapes.push_back(new Triangle(
		Vector3(2, 0, 2),
		Vector3(2, 0, -2),
		Vector3(-2, 0, -2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));

	// Ceiling
	shapes.push_back(new Triangle(
		Vector3(-2, 4, -2),
		Vector3(-2, 4, 2),
		Vector3(2, 4, 2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));
	shapes.push_back(new Triangle(
		Vector3(2, 4, 2),
		Vector3(2, 4, -2),
		Vector3(-2, 4, -2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));

	// Front wall
	shapes.push_back(new Triangle(
		Vector3(-2, 0, -2),
		Vector3(-2, 0, 2),
		Vector3(-2, 4, 2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));
	shapes.push_back(new Triangle(
		Vector3(-2, 4, 2),
		Vector3(-2, 4, -2),
		Vector3(-2, 0, -2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));

	// Left wall
	shapes.push_back(new Triangle(
		Vector3(-2, 0, -2),
		Vector3(2, 0, -2),
		Vector3(2, 4, -2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));
	shapes.push_back(new Triangle(
		Vector3(2, 4, -2),
		Vector3(-2, 4, -2),
		Vector3(-2, 0, -2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));

	// Right wall
	shapes.push_back(new Triangle(
		Vector3(-2, 0, 2),
		Vector3(2, 0, 2),
		Vector3(2, 4, 2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));
	shapes.push_back(new Triangle(
		Vector3(2, 4, 2),
		Vector3(-2, 4, 2),
		Vector3(-2, 0, 2),
		new Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	));

	shapes.push_back(new Sphere(
		Vector3(-1, 1, 1), 1, 
		new Metal(Vector3(1, 1, 1))
	));

	shapes.push_back(new Sphere(Vector3(1, 0.2, -1), 0.2, new Diffuse(Vector3(0.7f, 0.7f, 0.7f))));

	shapes.push_back(new Sphere(
		Vector3(0, 4, 0), 1, 
		new Lamp(Vector3(1, 1, 1))
	));

	// shapes.push_back(new Triangle(
	// 	Vector3(-0.3, 3.9, -0.3),
	// 	Vector3(-0.3, 3.9, 0.3),
	// 	Vector3(0.3, 3.9, 0.3),
	// 	new Lamp(Vector3(10, 10, 10))
	// ));
	// shapes.push_back(new Triangle(
	// 	Vector3(0.3, 3.9, 0.3),
	// 	Vector3(0.3, 3.9, -0.3),
	// 	Vector3(-0.3, 3.9, -0.3),
	// 	new Diffuse(Vector3(10, 10, 10))
	// ));
}

void initScene1(Scene& scene) {
	auto& shapes = scene.shapes;
	shapes.push_back(new Sky());

	for(int i = -11; i < 11; ++i) {
		for(int j = -11; j < 11; ++j) {
			Vector3 c(i + 0.9f * Math::random(), 0.2f, j + 0.9f * Math::random());
			float p = Math::random();
			if(p < 0.8f) {
				shapes.push_back(new Sphere(c, 0.2f, new Diffuse(Vector3(Math::random(), Math::random(), Math::random()))));
			} else if(p < 0.95f) {
				float r = 0.5f * (1 + Math::random());
				float g = 0.5f * (1 + Math::random());
				float b = 0.5f * (1 + Math::random());
				shapes.push_back(new Sphere(c, 0.2f, new Metal(Vector3(r, g, b), 0.5f * Math::random())));
			} else {
				shapes.push_back(new Sphere(c, 0.2f, new Dielectric(1.5f)));
			}
		}
	}
	shapes.push_back(new Sphere(Vector3(0, 1, 0), 1, new Dielectric(1.5f)));
	shapes.push_back(new Sphere(Vector3(-4, 1, 0), 1, new Diffuse(Vector3(0.4f, 0.2f, 0.1f))));
	shapes.push_back(new Sphere(Vector3(4, 1, 0), 1, new Metal(Vector3(0.7f, 0.6f, 0.5f))));
	shapes.push_back(new Sphere(Vector3(0, -1000, 0), 1000, new Diffuse(Vector3(0.5f))));
	//shapes.push_back(new Sphere(Vector3(0, 100, 0), 20, new Lamp(Vector3(20.0f))));

	float size = 13;
	Vector3 x1(-size, -1, -size);
	Vector3 x2(-size, -1, size);
	Vector3 x3(size, -1, size);
	Vector3 x4(size, -1, -size);
	Vector3 h(0, 5, 0);
	Vector3 color1(0.1f, 0.9f, 0.1f);
	Vector3 color2(0.9f, 0.1f, 0.1f);
	Vector3 color3(0.9f, 0.9f, 0.9f);
	Vector3 color4(0.1f, 0.1f, 0.9f);

	shapes.push_back(new Triangle(
		x1, x2, x1 + h, new Diffuse(color1)
	));
	shapes.push_back(new Triangle(
		x1+h, x2, x2 + h, new Diffuse(color1)
	));

	shapes.push_back(new Triangle(
		x2, x3, x2 + h, new Diffuse(color2)
	));
	shapes.push_back(new Triangle(
		x2+h, x3, x3 + h, new Diffuse(color2)
	));

	shapes.push_back(new Triangle(
		x3, x4, x3 + h, new Diffuse(color3)
	));
	shapes.push_back(new Triangle(
		x3+h, x4, x4 + h, new Diffuse(color3)
	));

	shapes.push_back(new Triangle(
		x1, x4, x1 + h, new Diffuse(color4)
	));
	shapes.push_back(new Triangle(
		x1+h, x4, x4 + h, new Diffuse(color4)
	));
}

int main() {
	Scene scene;
	initScene1(scene);

	Film film(500, 250);

	Camera camera(film, 20, 0, 12);
	camera.position = Vector3(12, 3, 5);
	camera.lookAt(Vector3(0, 1, 0));
	//camera.position = Vector3(15, 2, 0);
	//camera.lookAt(Vector3(0, 2, 0));

	Renderer renderer(10);
	renderer.render(scene, camera);

	film.gammaCorrection();
	film.writePPM();

	return 0;
}