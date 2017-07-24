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
			const I& z,
			const void* data) {
			I num = *((float*)(data));
			I x2 = x*x;
			I y2 = y*y;
			I z2 = z*z;
			return x2*x2 - I(5)*x2 + y2*y2 - I(5)*y2 + z2*z2 - I(5)*z2 + num;
		}

		static inline void normal(
			const I& x, 
			const I& y, 
			const I& z,
			I& xn, I& yn, I& zn,
			const void* data) {
			xn = I(4.0)*x*x*x - I(10.0)*x;
			yn = I(4.0)*y*y*y - I(10.0)*y;
			zn = I(4.0)*z*z*z - I(10.0)*z;
		}
	};
}

#endif