#include "render/math.h"
#include "render/film.h"
#include "render/camera.h"
#include "render/renderer.h"

#include "objects/all.h"
#include "materials/all.h"

using namespace gui;

template<class T>
void rect(std::vector<Shape*>& shapes, Vector3 x1, Vector3 x2, Vector3 x3, T material) {
	T* newMaterial = new T;
	*newMaterial = material;
	shapes.push_back(new Triangle(x1, x2, x3, newMaterial));

	newMaterial = new T;
	*newMaterial = material;
	shapes.push_back(new Triangle(x1, x2 - (x2-(x3+x1)*0.5f)*2, x3, newMaterial));
}

void initScene(Scene& scene) {
	auto& shapes = scene.shapes;
	// FLoor
	rect(shapes, 
		Vector3(-2, 0, -2),
		Vector3(-2, 0, 2),
		Vector3(20, 0, 2),
		Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	);

	// Ceiling
	rect(shapes, 
		Vector3(-2, 4, -2),
		Vector3(-2, 4, 2),
		Vector3(20, 4, 2),
		Diffuse(Vector3(0.9f, 0.9f, 0.9f))
	);

	// Front wall
	rect(shapes, 
		Vector3(-2, 0, -2),
		Vector3(-2, 0, 2),
		Vector3(-2, 4, 2),
		Diffuse(Vector3(0.7f, 0.7f, 0.7f))
	);

	// Left wall
	rect(shapes, 
		Vector3(-2, 0, -2),
		Vector3(20, 0, -2),
		Vector3(20, 4, -2),
		Diffuse(Vector3(0.9f, 0.1f, 0.1f))
	);

	// Right wall
	rect(shapes, 
		Vector3(-2, 0, 2),
		Vector3(20, 0, 2),
		Vector3(20, 4, 2),
		Diffuse(Vector3(0.1f, 0.9f, 0.1f))
	);

	shapes.push_back(new Sphere(
		Vector3(-1, 1, 1), 1, 
		new Metal(Vector3(1, 1, 1))
	));

	shapes.push_back(new Sphere(Vector3(-1.3f, 0.7f, -1.3f), 0.7f, new Diffuse(Vector3(0.1f, 0.1f, 0.9f))));
	shapes.push_back(new Sphere(Vector3(1, 0.5, 0.2), 0.5, new Dielectric(1.5f)));

	float lampSize = 1;
	rect(shapes, 
		Vector3(-lampSize, 3.95, -lampSize),
		Vector3(-lampSize, 3.95, lampSize),
		Vector3(lampSize, 3.95, lampSize),
		Lamp(Vector3(1))
	);
}

int main() {
	Scene scene;
	initScene(scene);

	Film film(1024, 1024);

	Camera camera(film, 20, 0, 12);
	camera.position = Vector3(15, 2, 0);
	camera.lookAt(Vector3(0, 2, 0));

	Renderer renderer(2000);
	renderer.render(scene, camera);

	film.gammaCorrection();
	film.writePPM("image3.ppm");

	return 0;
}