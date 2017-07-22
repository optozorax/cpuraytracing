#ifndef GUI_IMPLICIT_H
#define GUI_IMPLICIT_H

#include "materials/material.h"
#include "objects/shape.h"
#include "render/intersection.h"
#include "render/ray.h"
#include "objects/transformation.h"

#include "interval.h"

namespace gui {
	typedef interval<float> Inter;

	template <template<class> class F>
	class Implicit : public Shape {
	public:
		Implicit(Material* material, Transformation* tr, void* data) : Shape(material), tr(tr), data(data) { }

		~Implicit() { }

		bool intersect(
			const Ray& ray,
			float tMin,
			float tMax,
			Intersection& intersection
		) const;
	private:
		Transformation* tr;
		void* data;

		template <class I> inline I f(const I& t, const Vector3& oirigin, const Vector3& direction);
		template <class I> inline I f_diff(const I& t, const Vector3& origin, const Vector3& direction);
		float DichotomyMethod(float a, float b, float& t, const Vector3& o, const Vector3& d);
		bool NewtonMethod(float x, float min, float max, float& t, const Vector3& o, const Vector3& d);
		bool ray_intersect(const Inter t, float& t1, int depth, const Vector3& o, const Vector3& d);
	};

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	template <template<class> class F>
	template <class I>
	inline I Implicit<F>::f(const I& t, const Vector3& origin, const Vector3& direction) { 
		I x = I(origin.x) + I(direction.x) * t;
		I y = I(origin.y) + I(direction.y) * t;
		I z = I(origin.z) + I(direction.z) * t;
		return F<I>::function(x, y, z, data); 
	}

	template <template<class> class F>
	template <class I>
	inline I Implicit<F>::f_diff(const I& t, const Vector3& origin, const Vector3& direction) {
		// returns df(origin + direction * t)/dt
		I x = I(origin.x) + I(direction.x) * t;
		I y = I(origin.y) + I(direction.y) * t;
		I z = I(origin.z) + I(direction.z) * t;

		I xn, yn, zn;
		F<I>::normal(x, y, z, xn, yn, zn, data);

		I xd = I(direction.x);
		I yd = I(direction.y);
		I zd = I(direction.z);
		return xn*xd + yn*yd + zn*zd;
	}

	template <template<class> class F>
	bool Implicit<F>::NewtonMethod(float x, float min, float max, float& t, const Vector3& o, const Vector3& d) {
		const int maxIter = 20;
		const float eps = 0.00005f;

		for (int i = 0; i < maxIter; ++i) {
			x = x - f<float>(x, o, d)/f_diff<float>(x, o, d);
			if (fabs(f<float>(x, o, d)) < eps*x) {
				t = x;
				return true;
			}

			if (x > max || x < min)
				return false;
		}

		return false;
	}

	template <template<class> class F>
	float Implicit<F>::DichotomyMethod(float a, float b, float& t, const Vector3& o, const Vector3& d) {
		const int maxIter = 50;
		const float eps = 0.000005f;
		float fa = f<float>(a, o, d);
		float fb = f<float>(b, o, d);
		float c, fc;

		if (fa*fb > 0) return false;

		for (int i = 0; i < maxIter; ++i) {
			c = (a+b)/2.0f;
			fc = f<float>(c, o, d);

			if (fa*fc < 0) {
				fb = fc;
				b = c;
			} else {
				fa = fc;
				a = c;
			};

			if (fabs(b-a) < eps*a)
				return NewtonMethod(c, a, b, t, o, d);;
		}

		return false;
	}

	template <template<class> class F>
	bool Implicit<F>::ray_intersect(const Inter t, float& t1, int depth, const Vector3& o, const Vector3& d) {
		// Write in RES most left interval which has root, and when function is monotone.
		// Return true if this interval exist on t.
		// Monotone == function has one root on interval.
		// Function has root on [a, b], when f(a)*f(b) < 0,
		//      i.e. exist c in [a, b] such that  f(c) = 0.
		/* For more information read: https://pdfs.semanticscholar.org/fc57/b015acfc62c54a4bd82ca1ef1e3ef4d0c710.pdf
		   (Ray Tracing Implicit Surfaces; John C. Hart; School of EECS; Washington State University; Pullman, WA 99164-2752; hart@eecs.wsu.edu)
		   on page 10. */
		if (depth > 50)  return false;
		Inter s, r, t2;

		s = f<Inter>(t, o, d);
		if (!zero_in(s)) {
			return false;
		} else {
			r = f_diff<Inter>(t, o, d);
			if (!zero_in(r)) {
				return DichotomyMethod(t.lower(), t.upper(), t1, o, d);
				//return NewtonMethod((t.lower()+t.upper())/2.0f, t.lower(), t.upper(), t1, o, d);
			} else {
				t2 = Inter(t.lower(), (t.lower()+t.upper())/2.0f);
				if (ray_intersect(t2, t1, ++depth, o, d)) 
					return true;
				else {
					t2 = Inter((t.lower()+t.upper())/2.0f, t.upper());
					return ray_intersect(t2, t1, ++depth, o, d); 
				}
			}
		}
	}

	template <template<class> class F>
	bool Implicit<F>::intersect(
		const Ray& ray,
		float tMin,
		float tMax,
		Intersection& intersection
	) const {
		Vector3 origin = ray.origin;
		Vector3 direction = ray.direction;
		tr->transform(origin, direction);

		float t;

		if (ray_intersect(Inter(tMin, tMax), t, 0, origin, direction)) {
			intersection.t = t;
			intersection.position = origin + direction * t;

			float x, y, z;
			F<float>::normal(
				intersection.position.x, 
				intersection.position.y, 
				intersection.position.z, 
				x, y, z, data);
			intersection.normal = Vector3(x, y, z).identity();
			tr->inverse(intersection.position, intersection.normal);

			intersection.material = material;

			return true;
		} else
			return false;
	};
}

#endif