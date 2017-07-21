#include <boost/numeric/interval.hpp>

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

using namespace gui;

using namespace boost;
using namespace numeric;
using namespace interval_lib;

// class Torus : public Shape {
// public:
// 	Torus(Material* material = new Diffuse(Vector3(0.1f, 0.9f, 0.1f))) : 
// 		Shape(material),
// 		s1(Vector3(0, 1, 0), 1, new Diffuse(Vector3(0.1f, 0.9f, 0.1f))),
// 		s2(Vector3(0, 2, 0), 1, new Diffuse(Vector3(0.1f, 0.9f, 0.1f))) {
// 	}

// 	bool intersect(
// 		const Ray& ray,
// 		float tMin,
// 		float tMax,
// 		Intersection& intersection
// 	) const {
// 		Intersection int1 = {}, int2 = {};
// 		bool b1 = s1.intersect(ray, tMin, tMax, int1);
// 		bool b2 = s2.intersect(ray, tMin, tMax, int2);
// 		if (b1 && b2) {
// 			if (int1.t > int2.t) {
// 				intersection = int1;
// 				return b1;
// 			} else {
// 				intersection = int2;
// 				return b2;
// 			}
// 		} else 
// 		return false;
// 	};

// private:
// 	Sphere s1, s2;
// };

// typedef union {
// 	float f;
// 	struct {
// 		unsigned int mantisa : 23;
// 		unsigned int exponent : 8;
// 		unsigned int sign : 1;
// 	} parts;
// } float_cast;

// float getEps(float a) {
// 	const int bitNo = 23-10;
// 	float_cast d1 = { .f = a };
// 	d1.parts.mantisa = 0;
// 	if (d1.parts.exponent < bitNo)
// 		d1.parts.exponent = 1;
// 	else
// 		d1.parts.exponent -= bitNo;
// 	return d1.f;
// }

// #define max(a,b) (((a)>(b)) ? (a) : (b))

// float f(Vector3 p) {
// 	//return p.length()-1;

// 	float x = p.x;
// 	float y = p.y-2;
// 	float z = p.z;
// 	float R = 1;
// 	float r = 1.0f/3.0f;
// 	return pow((R-sqrt(x*x+y*y)),2.0)+z*z-r*r;
// 	//return 1/(x-y*y)-z;
// 	//return x*x + y*y - z*z + 1;
// 	//return x*x-y*y-z;
// }

// Vector3 getNormal(Vector3 p) {
// 	float maximum = max(max(p.x, p.y),p.z);
// 	float eps = getEps(maximum);
// 	Vector3 z1 = p + Vector3(eps, 0, 0);
// 	Vector3 z2 = p - Vector3(eps, 0, 0);
// 	Vector3 z3 = p + Vector3(0, eps, 0);
// 	Vector3 z4 = p - Vector3(0, eps, 0);
// 	Vector3 z5 = p + Vector3(0, 0, eps);
// 	Vector3 z6 = p - Vector3(0, 0, eps);

// 	float dx = f(z1) - f(z2);
// 	float dy = f(z3) - f(z4);
// 	float dz = f(z5) - f(z6);

// 	return (Vector3(dx, dy, dz) / (2.0*eps));

// 	//return Vector3(2*p.x, 2*p.y, -2*p.z);

// 	// float x = p.x;
// 	// float y = p.y-2;
// 	// float z = p.z;
// 	// float R = 1;
// 	// float r = 1.0f/3.0f;
// 	// return Vector3(
// 	// 	2*x*(1-R/sqrt(x*x+y*y)),
// 	// 	2*y*(1-R/sqrt(x*x+y*y)),
// 	// 	2*z);
// }

bool NewtonMethod(
	const Ray& ray,
	float tMin,
	float tMax,
	float& tRes) {
	const int maxIter = 10;
	const float eps = 0.00005;

	Vector3 ori = ray.origin;
	Vector3 dir = ray.direction;
	Vector3 pos;
	float t = 0;

	pos = ori + dir*t;
	for (int i = 0; i < maxIter; ++i) {
		t = t - f(pos)/(getNormal(pos).dot(dir));
		pos = ori + dir*t; 

		if (t > tMax && t < tMin) 
			return false;

		if (fabs(f(pos)) < eps*t) {
			tRes = t;
			return true;
		}
	}
	
	return false;
}

template <class I> I f(const Vector3& p) { 
	I x = p.x;
	I y = p.y;
	I z = p.z;
	I R = I(1.0);
	I r = I(1.0/3.0);
	return pow((R-sqrt(x*x+y*y)),I(2.0))+z*z-r*r; 
}
Vector3 getNormal(const Vector3& p) {
	I x = p.x;
	I y = p.y;
	I z = p.z;
	I R = I(1.0);
	I r = I(1.0/3.0);
	return Vector3(
		2*x*(1-R/sqrt(x*x+y*y)),
		2*y*(1-R/sqrt(x*x+y*y)),
		2*z);
}
template <class I> I f_diff(const Vector3& p, const Vector3& d) {
	Vector3 normal = getNormal(p);
	I x = d.x;
	I y = d.y;
	I z = d.z;
	return I(normal.x)*x + I(normal.y)*y + I(normal.z)*z;
}

using namespace boost;
using namespace numeric;
using namespace interval_lib;

typedef interval<double> Inter;

template<class T, class Policies>
std::ostream &operator<<(std::ostream &os,
                         const boost::numeric::interval<T, Policies> &x) {
  os << "[" << x.lower() << ", " << x.upper() << "]";
  return os;
}

bool ray_intersect(const Inter t, Inter& res) {
  Inter s, r, t1;

  s = f(t);
  if (!zero_in(s)) {
    return false;
  } else {
    r = f_diff(t);
    if (!zero_in(r)) {
      res = t;
      return true;
    } else {
      t1 = Inter(t.lower(), (t.lower()+t.upper())/2.0);
      if (ray_intersect(t1, res, o, d)) 
        return true;
      else {
        t1 = Inter((t.lower()+t.upper())/2.0, t.upper());
        return ray_intersect(t1, res, o, d); 
      }
    }
  }
}

class Torus : public Shape {
public:
	Torus(Material* material = new Diffuse(Vector3(0.1f, 0.9f, 0.1f))) : 
		Shape(material) {
	}

	bool intersect(
		const Ray& ray,
		float tMin,
		float tMax,
		Intersection& intersection
	) const {
		// const int maxIter = 64;
		// const float eps = 0.00005;
		// const int maxChances = 15;

		// int approxSize = 15;

		// Vector3 ori = ray.origin;
		// Vector3 dir = ray.direction;
		// Vector3 pos;
		// float t = 0;

		// float globalT = tMax;
		// for (int j = 0; j < maxChances; ++j) {
		// 	t = (float(j))/maxChances * approxSize;
		// 	pos = ori + dir*t;
		// 	for (int i = 0; i < maxIter; ++i) {
		// 		t = t - f(pos)/(getNormal(pos).dot(dir));
		// 		pos = ori + dir*t; 

		// 		if (t > tMax && t < tMin) 
		// 			break;

		// 		if (fabs(f(pos)) < eps*t) {
		// 			if (t < globalT) 
		// 				globalT = t;
		// 			break;
		// 		}
		// 	}
		// }

		// if (t > tMax && t < tMin) 
		// 		return false;

		// if (fabs(f(ori + dir * globalT)) < eps*globalT) {
		// 	intersection.t = globalT;
		// 	intersection.position = ori + dir * globalT;
		// 	intersection.normal = getNormal(ori + dir * globalT).identity();
		// 	intersection.material = material;
		// 	return true;
		// }
		// return false;

		//---------------------------------------------------------------------------------
		Inter i;
		if (ray_intersect(Inter(tMin, tMax), i, ray.origin, ray.direction)) {
			float t = 0;
			if (NewtonMethod(ray, tMin, tMax, t)) {
				intersection.t = t;
				intersection.position = ray.origin + ray.direction * t;
				intersection.normal = getNormal(intersection.position).identity();
				intersection.material = material;
			} else
				return false;
		} else
			return false;

		//----------------------------------------------------------------------------------

		// const int maxIter = 64;
		// const float eps = 0.00005;

		// Vector3 ori = ray.origin;
		// Vector3 dir = ray.direction;
		// Vector3 pos;
		// float t = 0;

		// pos = ori + dir*t;
		// float sign = f(pos);
		// sign /= fabs(sign);
		// for (int i = 0; i < maxIter; ++i) {
		// 	t = t + sign*f(pos);
		// 	pos = ori + dir*t; 

		// 	if (t > tMax && t < tMin) 
		// 		return false;

		// 	if (fabs(f(pos)) < eps*t) {
		// 		intersection.t = t;
		// 		intersection.position = pos;
		// 		intersection.normal = getNormal(pos).identity();
		// 		intersection.material = material;
		// 		return true;
		// 	}
		// }
		
		// return false;
	};
};

void initScene(Scene& scene) {
	auto& shapes = scene.shapes;
	shapes.push_back(new Sky());
	shapes.push_back(new Torus(new Metal(Vector3(0.9f), 0)));
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

	float time = Time::getCurrentTime();
	int count = 0;
	//#pragma omp parallel for shared(scene, count) num_threads(3)
	//for (int i = 0; i < 360; ++i) {
	int i = 240; {
		float alpha = i/180.0 * Math::PI;

		Film film(100, 50);

		Camera camera(film, 15, 0, 12);
		camera.position = Vector3(cos(alpha)*r, fabs(sin(alpha)*h)+1, sin(alpha)*r);
		camera.lookAt(Vector3(0, 2, 0));

		Renderer renderer(3);

		Ray ray1 = camera.getRay(44, 29);
		printf("o: %.5f %.5f %.5f; d: %.5f %.5f %.5f;\n", ray1.origin.x, ray1.origin.y, ray1.origin.z, ray1.direction.x, ray1.direction.y, ray1.direction.z);
		Vector3 color = renderer.radiance(ray1, scene, 0);
		printf("color: %.3f %.3f %.3f\n", color.x, color.y, color.z);
		// ray1 = camera.getRay(935, 1371);
		// renderer.radiance(ray1, scene, 0);

		renderer.render(scene, camera);

		film.gammaCorrection();

		char s[100] = {};
		sprintf(s,"imgs/image%3d.bmp", i);
		film.writeBMP(s);

		#pragma omp critical (section1)
		{
			count++;
			std::string timePassed = Time::getTimeString(Time::getTimePassed(time));
			std::string timeApprox = Time::getTimeString(Time::getApproxTime(Time::getTimePassed(time), count/360.0f));
			std::string timeLeft = Time::getTimeString(Time::getLeftTime(Time::getTimePassed(time), count/360.0f));
			printf("Count: %3d; Curr. time: %s; Approx. time: %s; Time left: %s\n", 
				count, 
				timePassed.c_str(),
				timeApprox.c_str(),
				timeLeft.c_str());
		}
	}
	std::string timeTotal = Time::getTimeString(Time::getTimePassed(time));
	printf("Total time: %s", timeTotal.c_str());

	return 0;
}