#ifndef GUI_TRANSFORMATION_H
#define GUI_TRANSFORMATION_H

#include <math.h>

#include "render/Vector3.h"
#include "render/math.h"

namespace gui {
	class Transformation {
	public:
		Transformation(Transformation* tr = nullptr) : tr(tr) {}

		~Transformation() {
			if (tr != nullptr)
				delete tr;
		}

		virtual void transform(
			const Vector3& origin,
			const Vector3& direction
		) = 0;

		virtual void inverse(
			const Vector3& origin,
			const Vector3& direction
		) = 0;

		Transformation* tr;
	};

	class TrMove : public Transformation {
	public:
		TrMove(Vector3 c, Transformation* tr) : Transformation(tr), c(c) {}

		void transform(
			const Vector3& origin,
			const Vector3& direction
		) {
			if (tr != nullptr) 
				tr->transform(origin, direction);

			origin -= c;
		};

		void inverse(
			const Vector3& origin,
			const Vector3& direction
		) {
			origin += c;

			if (tr != nullptr) 
				tr->inverse(origin, direction);
		};

	private:
		Vector3 c;
	};

	class TrScale : public Transformation {
	public:
		TrScale(Vector3 c, Transformation* tr) : Transformation(tr), c(c) {}

		void transform(
			const Vector3& origin,
			const Vector3& direction
		) {
			if (tr != nullptr) 
				tr->transform(origin, direction);

			origin /= c;
			direction /= c;
		};

		void inverse(
			const Vector3& origin,
			const Vector3& direction
		) {
			origin *= c;
			direction *= c;

			if (tr != nullptr) 
				tr->inverse(origin, direction);
		};

	private:
		Vector3 c;
	};

	class TrRotate : public Transformation {
	public:
		rotate2(float& x, float &y, float angle) {
			float x1 = cos(angle)*x - sin(angle)*y;
			float y1 = sin(angle)*x + cos(angle)*y;
			x = x1;
			y = y1;
		}

		rotate3(Vector3& a, Vector3 angles) {
			rotate2(a.x, a.y, angles.z);
			rotate2(a.x, a.z, angles.y);
			rotate2(a.y, a.z, angles.x);
		}

		rotate3i(Vector3& a, Vector3 angles) {
			rotate2(a.y, a.z, -angles.x);
			rotate2(a.x, a.z, -angles.y);
			rotate2(a.x, a.y, -angles.z);
		}

		TrRotate(Vector3 angles, Transformation* tr) : Transformation(tr), angles(angles) { }

		void transform(
			const Vector3& origin,
			const Vector3& direction
		) {
			if (tr != nullptr) 
				tr->transform(origin, direction);

			Vector3 orig = origin;
			Vector3 ordirection = origin + direction;

			rotate3(orig, angles);
			rotate3(ordirection, angles);
			origin = orig;
			direction = (ordirection-origin).identity();
		};

		void inverse(
			const Vector3& origin,
			const Vector3& direction
		) {
			Vector3 orig = origin;
			Vector3 ordirection = origin + direction;

			rotate3i(orig, angles);
			rotate3i(ordirection, angles);
			origin = orig;
			direction = (ordirection-origin).identity();

			if (tr != nullptr) 
				tr->inverse(origin, direction);
		};

	private:
		Vector3 angles;
		Vector3 m1, m2, m3;
		Vector3 n1, n2, n3;
	};
}

#endif