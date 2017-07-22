#ifndef GUI_IMPLICIT_F_H
#define GUI_IMPLICIT_F_H

//#include <iostream>

namespace gui {
	template <class I>
	class Torus {
	public:
		static inline I function(
			const I& x, 
			const I& y, 
			const I& z,
			const void* data) { 
			I R = ((float*)(data))[0];
			I r = ((float*)(data))[1];
			//std::cout << "data: " << ((float*)(data))[0] << " " << ((float*)(data))[1] << std::endl;
			return (x*x + y*y + z*z + R*R - r*r)*(x*x + y*y + z*z + R*R - r*r) - I(4.0)*R*R*(x*x + y*y);
		}

		static inline void normal(
			const I& x, 
			const I& y, 
			const I& z,
			I& xn, I& yn, I& zn,
			const void* data) {
			I R = ((float*)(data))[0];
			I r = ((float*)(data))[1];

			I xyzr = x*x + y*y + z*z + R*R - r*r;
			xn = I(4.0)*x*xyzr - I(8.0)*R*R*x;
			yn = I(4.0)*y*xyzr - I(8.0)*R*R*y;
			zn = I(4.0)*z*xyzr;
		}
	};

	template <class I>
	class Tangle {
	public:
		static inline I function(
			const I& x, 
			const I& y, 
			const I& z) {
			I x2 = x*x;
			I y2 = y*y;
			I z2 = z*z;
			return x2*x2 - I(5)*x2 + y2*y2 - I(5)*y2 + z2*z2 - I(5)*z2 + I(11.8);
		}

		static inline void normal(
			const I& x, 
			const I& y, 
			const I& z,
			I& xn, I& yn, I& zn) {
			xn = I(4.0)*x*x*x - I(10.0)*x;
			yn = I(4.0)*y*y*y - I(10.0)*y;
			zn = I(4.0)*z*z*z - I(10.0)*z;
		}
	};

	template <class I>
	class SuSE95 {
	public:
		static inline I function(
			const I& x, 
			const I& y, 
			const I& z) {
			I t = I((1.0+sqrt(5.0))/2.0);
			return I(4)*(t*t*x*x-y*y)*(t*t*y*y-z*z)*(t*t*z*z-x*x)-(I(1)+I(2)*t)*(x*x+y*y*z*z-I(1))*(x*x+y*y*z*z-I(1));
		}

		static inline void normal(
			const I& x, 
			const I& y, 
			const I& z,
			I& xn, I& yn, I& zn) {
			I t = I((1.0+sqrt(5.0))/2.0);
			I tyz = t*t*y*y-z*z;
			I txy = t*t*x*x-y*y;
			I tzx = t*t*z*z-x*x;
			I a = -I(4)*(I(2)*t+I(1))*(y*y*z*z+x*x-I(1));
			xn = a*x + I(8)*t*t*x*tyz*tzx - I(8)*x*txy*tyz;
			yn = a*y*z*z - I(8)*y*tyz*tzx + I(8)*t*t*y*txy*tzx;
			zn = a*y*y*z - I(8)*z*txy*tzx + I(8)*t*t*z*txy*tyz;
		}
	};
}

#endif