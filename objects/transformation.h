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
		TrMove(Transformation* tr, Vector3 c) : Transformation(tr), c(c) {}

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

	class TrRotate : public Transformation {
	public:
		TrRotate(Transformation* tr, Vector3 angles) : Transformation(tr), angles(angles) {
			m1.x = cos(angles.x)*cos(angles.z) - sin(angles.x)*cos(angles.y)*sin(angles.z);
			m1.y = -cos(angles.x)*sin(angles.z) - sin(angles.x)*cos(angles.y)*cos(angles.z);
			m1.z = sin(angles.x)*sin(angles.y);

			m2.x = sin(angles.x)*cos(angles.z) + cos(angles.x)*cos(angles.y)*sin(angles.z);
			m2.y = -sin(angles.x)*sin(angles.z) + cos(angles.x)*cos(angles.y)*cos(angles.z);
			m2.z = -cos(angles.x)*sin(angles.y);

			m3.x = sin(angles.y)*sin(angles.z);
			m3.y = sin(angles.y)*cos(angles.z);
			m3.z = cos(angles.y);

			n1.x = cos(angles.z)*cos(angles.x) - sin(angles.z)*cos(angles.y)*sin(angles.x);
			n1.y = cos(angles.z)*sin(angles.x) + sin(angles.z)*cos(angles.y)*cos(angles.x);
			n1.z = sin(angles.z)*sin(angles.y);

			n2.x = -sin(angles.z)*cos(angles.x) - cos(angles.z)*cos(angles.y)*sin(angles.x);
			n2.y = -sin(angles.z)*sin(angles.x) + cos(angles.z)*cos(angles.y)*cos(angles.x);
			n2.z = cos(angles.z)*sin(angles.y);

			n3.x = sin(angles.y)*sin(angles.x);
			n3.y = -sin(angles.y)*cos(angles.x);
			n3.z = cos(angles.y);
		}

		void transform(
			const Vector3& origin,
			const Vector3& direction
		) {
			if (tr != nullptr) 
				tr->transform(origin, direction);

			Vector3 ordirection = origin + direction;

			origin = Vector3(m1.dot(origin), m2.dot(origin), m3.dot(origin));
			ordirection = Vector3(m1.dot(ordirection), m2.dot(ordirection), m3.dot(ordirection));
			direction = (ordirection-origin).identity();
		};

		void inverse(
			const Vector3& origin,
			const Vector3& direction
		) {
			Vector3 ordirection = origin + direction;

			origin = Vector3(n1.dot(origin), n2.dot(origin), n3.dot(origin));
			ordirection = Vector3(n1.dot(ordirection), n2.dot(ordirection), n3.dot(ordirection));
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