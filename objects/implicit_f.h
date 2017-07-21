#ifndef GUI_IMPLICIT_F_H
#define GUI_IMPLICIT_F_H

namespace gui {
	template <class I>
	class Torus {
	public:
		static inline I function(
			const I& x, 
			const I& y, 
			const I& z) { 
			I R = 1.0;
			I r = 1.0/3.0;

			return (x*x + y*y + z*z + R*R - r*r)*(x*x + y*y + z*z + R*R - r*r) - I(4.0)*R*R*(x*x + y*y);
		}

		static inline void normal(
			const I& x, 
			const I& y, 
			const I& z,
			I& xn, I& yn, I& zn) {
			I R = 1.0;
			I r = 1.0/3.0;

			I xyzr = x*x + y*y + z*z + R*R - r*r;
			xn = I(4.0)*x*xyzr - I(8.0)*R*R*x;
			yn = I(4.0)*y*xyzr - I(8.0)*R*R*y;
			zn = I(4.0)*z*xyzr;
		}
	};
}

#endif