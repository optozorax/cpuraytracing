//#include <boost/numeric/interval.hpp>
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

// typedef interval<float> Inter;

// template<class T>
// std::ostream &operator<<(std::ostream &os, const interval<T> &x) {
//   os << "[" << x.lower() << ", " << x.upper() << "]";
//   return os;
// }

// template <class I> inline I function(
// 	const I& x, 
// 	const I& y, 
// 	const I& z) { 
// 	//y = y - I(2.0);
// 	I R = I(1.0);
// 	I r = I(1.0/3.0);

// 	// return pow(R-sqrt(x*x+y*y), 2)+z*z-r*r; // 'f' from f(x,y,z) = 0
// 	return (x*x + y*y + z*z + R*R - r*r)*(x*x + y*y + z*z + R*R - r*r) - I(4.0)*R*R*(x*x + y*y);
// 	//return x*x + y*y + z*z - R*R;
// }

// template <class I> inline void normal(
// 	const I& x, 
// 	const I& y, 
// 	const I& z,
// 	I& xn, I& yn, I& zn) {
// 	//y = y - I(2.0);
// 	I R = I(1.0);
// 	I r = I(1.0/3.0);

// 	// xn = I(2.0)*x*(I(1.0)-R/sqrt(x*x+y*y)); // df(x,y,z)/dx
// 	// yn = I(2.0)*y*(I(1.0)-R/sqrt(x*x+y*y)); // df(x,y,z)/dy
// 	// zn = I(2.0)*z; // df(x,y,z)/dz

// 	I xyzr = x*x + y*y + z*z + R*R - r*r;
// 	xn = I(4.0)*x*xyzr - I(8.0)*R*R*x;
// 	yn = I(4.0)*y*xyzr - I(8.0)*R*R*y;
// 	zn = I(4.0)*z*xyzr;

// 	// xn = I(2.0)*x;
// 	// yn = I(2.0)*y;
// 	// zn = I(2.0)*z;
// }

// class Torus : public Shape {
// public:
// 	Torus(Material* material) : 
// 		Shape(material) {
// 	}

// 	Vector3 origin, direction;

// 	template <class I> inline I f(const I& t) { 
// 		I x = I(origin.x) + I(direction.x) * t;
// 		I y = I(origin.y) + I(direction.y) * t;
// 		I z = I(origin.z) + I(direction.z) * t;
// 		return function(x, y, z); 
// 	}

// 	template <class I> inline I f_diff(const I& t) {
// 		// returns df(origin + direction * t)/dt
// 		I x = I(origin.x) + I(direction.x) * t;
// 		I y = I(origin.y) + I(direction.y) * t;
// 		I z = I(origin.z) + I(direction.z) * t;

// 		I xn, yn, zn;
// 		normal(x, y, z, xn, yn, zn);

// 		I xd = I(direction.x);
// 		I yd = I(direction.y);
// 		I zd = I(direction.z);
// 		return xn*xd + yn*yd + zn*zd;
// 	}

// 	bool NewtonMethod(float x, float min, float max, float& t) {
// 		const int maxIter = 10;
// 		const float eps = 0.00005f;

// 		for (int i = 0; i < maxIter; ++i) {
// 			x = x - f<float>(x)/f_diff<float>(x);
// 			if (fabs(f<float>(x)) < eps*x) {
// 				t = x;
// 				return true;
// 			}

// 			if (x > max && x < min) 
// 				return false;
// 		}

// 		return false;
// 	}

// 	float DichotomyMethod(float a, float b, float& t) {
// 		const int maxIter = 50;
// 		const float eps = 0.0005f;
// 		float fa = f<float>(a);
// 		float fb = f<float>(b);
// 		float c, fc;

// 		if (fa*fb > 0) return false;

// 		for (int i = 0; i < maxIter; ++i) {
// 			c = (a+b)/2.0f;
// 			fc = f<float>(c);

// 			if (fa*fc < 0) {
// 				fb = fc;
// 				b = c;
// 			} else {
// 				fa = fc;
// 				a = c;
// 			};

// 			if (fabs(b-a) < eps*a) {
// 				t = c;
// 				return true;
// 			}
// 		}

// 		return false;
// 	}

// 	bool ray_intersect(const Inter t, float& t1, int depth) {
// 		// Write in RES most left interval which has root, and when function is monotone.
// 		// Return true if this interval exist on t.
// 		// Monotone == function has one root on interval.
// 		// Function has root on [a, b], when f(a)*f(b) < 0,
// 		//      i.e. exist c in [a, b] such that  f(c) = 0.
// 		/* For more information read: https://pdfs.semanticscholar.org/fc57/b015acfc62c54a4bd82ca1ef1e3ef4d0c710.pdf
// 		   (Ray Tracing Implicit Surfaces; John C. Hart; School of EECS; Washington State University; Pullman, WA 99164-2752; hart@eecs.wsu.edu)
// 		   on page 10. */
// 		if (depth > 50)  return false;
// 		Inter s, r, t2;

// 		s = f<Inter>(t);
// 		if (!zero_in(s)) {
// 			return false;
// 		} else {
// 			r = f_diff<Inter>(t);
// 			if (!zero_in(r)) {
// 				//return DichotomyMethod(t.lower(), t.upper(), t1);
// 				return NewtonMethod((t.lower()+t.upper())/2.0f, t.lower(), t.upper(), t1);
// 			} else {
// 				t2 = Inter(t.lower(), (t.lower()+t.upper())/2.0f);
// 				if (ray_intersect(t2, t1, ++depth)) 
// 					return true;
// 				else {
// 					t2 = Inter((t.lower()+t.upper())/2.0f, t.upper());
// 					return ray_intersect(t2, t1, ++depth); 
// 				}
// 			}
// 		}
// 	}

// 	bool intersect(
// 		const Ray& ray,
// 		float tMin,
// 		float tMax,
// 		Intersection& intersection
// 	) const {
// 		origin = ray.origin;
// 		direction = ray.direction;

// 		float t;
// 		if (ray_intersect(Inter(tMin, tMax), t, 0)) {
// 			intersection.t = t;
// 			intersection.position = ray.origin + ray.direction * t;

// 			float x, y, z;
// 			normal(intersection.position.x, 
// 				   intersection.position.y, 
// 				   intersection.position.z, 
// 				   x, y, z);
// 			intersection.normal = Vector3(x, y, z).identity();

// 			intersection.material = material;
// 			return true;
// 		} else
// 			return false;
// 	};
// };

void initScene(Scene& scene) {
	auto& shapes = scene.shapes;
	shapes.push_back(new Sky());
	//shapes.push_back(new Torus(new Metal(Vector3(0.9f), 0)));
	shapes.push_back(
		new Implicit<Torus>(new Metal(Vector3(0.9f), 0), 
		new TrRotate(
			new TrMove(nullptr, Vector3(0, 2, 0))
		, Vector3(0, 50, 20)/180.0f * Math::PI)
	));
	//shapes.push_back(new Sphere(Vector3(0, 3, 0), 1, new Diffuse(Vector3(0.9f, 0.1f, 0.9f))));
	for (int i = -5; i < 5; ++i) {
		for (int j = -5; j < 5; ++j) {
			shapes.push_back(new Sphere(Vector3(i, 0.3, j), 0.3, new Diffuse(Vector3(0.9f, abs(i+j)/10.0f, 0.9f))));
		}
	}
	shapes.push_back(new Sphere(Vector3(0, -1000, 0), 1000, new Diffuse(Vector3(0.8f))));
}

int main() {
	Scene scene;
	initScene(scene);

	const float r = 10;
	const float h = 10;

	float pastTime = Time::getCurrentTime();
	int count = 0;
	//#pragma omp parallel for shared(scene, count) num_threads(3)
	//for (int i = 0; i < 360; ++i) {
	int i = 240; {
		float alpha = i/180.0 * Math::PI;

		Film film(500, 250);

		Camera camera(film, 15, 0, 12);
		camera.position = Vector3(cos(alpha)*r, fabs(sin(alpha)*h)+1, sin(alpha)*r);
		camera.lookAt(Vector3(0, 2, 0));

		Renderer renderer(10);
		renderer.render(scene, camera);

		film.gammaCorrection();

		char s[100] = {};
		sprintf(s,"imgs/image%3d.bmp", i);
		film.writeBMP(s);

		#pragma omp critical (section1)
		{
			count++;
			std::cout << "\rCount: " << count;
			Time::writeTimeStatus(pastTime, count/360.0f);
		}
	}
	Time::writeTotalTime(pastTime);

	return 0;
}