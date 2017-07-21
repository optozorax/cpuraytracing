#ifndef GUI_INTERVAL_H
#define GUI_INTERVAL_H

#include <limits>

template<class T> class interval {
public:
	T const& lower(void) const { return a; }
	T const& upper(void) const { return b; }

	interval() {};
	interval(T const &v) : a(v), b(v) {}
	template<class T1> interval(T1 const &v) : a(T(v)), b(T(v)) {}
	interval(T const &l, T const &u) : a(l), b(u) {}
	template<class T1, class T2> interval(T1 const &l, T2 const &u) : a(T(l)), b(T(u)) {}

	interval operator-(void) const { 
		return interval(-b, -a); 
	}

	interval operator+(interval c) const { 
		return interval(c.a + a, c.b + b); 
	}
	interval operator-(interval c) const { 
		return interval(a - c.b, b - c.a); 
	}
	interval operator*(interval c) const  { 
		return interval(
			mymin(mymin(mymin(a*c.a, a*c.b), b*c.a), b*c.b), 
			mymax(mymax(mymax(a*c.a, a*c.b), b*c.a), b*c.b)
		); 
	}
	interval operator/(interval c) const { 
		if (!(c.lower() * c.upper() < 0))
			return interval(a, b) * interval(1/c.b, 1/c.a);
		else
			return interval(
				-std::numeric_limits<T>::infinity(), 
				 std::numeric_limits<T>::infinity()
			); 
	}
private:
	T a,b;

	T mymin(T a, T b) { return (a < b) ? a : b; }
	T mymax(T a, T b) { return (a > b) ? a : b; }
};

template<class T>
bool zero_in(interval<T> c) { return c.lower() * c.upper() < 0; }

#endif