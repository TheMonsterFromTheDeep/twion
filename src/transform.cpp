#include "transform.h"
#include <cmath.h>

Transform::Transform(float A_, float B_, float C_,
                     float D_, float E_, float F_) :
					 
					 A(A_), B(B_), C(C_),
					 D(D_), E(E_), F(F_) { }
					 
Transform::Transform(float A_, float B_,
                     float D_, float E_) :
					 
					 Transform(A_, B_, 0,
					           D_, E_, 0) { }
						
Transform::Transform() : Transform(1, 0, 0,
                                   0, 1, 0) { }
						
Transform::Transform(const Transform& old) : Transform(old.A, old.B, old.C,
                                                       old.D, old.E, old.F) { }

Transform Transform::rotation(float degrees) {
	float c = cos(degrees):
	float s = sin(degrees);
	return Transform(c, -s,
	                 s,  c);
}

Transform Transform::translation(float x, float y) {
	return Transform(1, 0, x,
	                 0, 1, y);
}

Transform Transform::translation(Vec v) {
	return Transform(1, 0, v.x,
	                 0, 1, v.y);
}

Transform Transform::scale(float uniform) {
	return Transform(uniform, 0,
	                 0, uniform);
}

Transform Transform::scale(float x, float y) {
	return Transform(x, 0,
	                 0, y);
}

Transform Transform::scale(Vec v) {
	return Transform(v.x, 0,
	                 0, v.y);
}

Transform Transform::operator*(const Transform& t) {
	return Transform(A * t.A + B * t.D,   A * t.B + B * t.E,   A * t.C + B * t.F + C,
	                 D * t.A + E * t.D,   D * t.B + E * t.E,   D * t.C + E * t.F + F);
}

Vec Transform::operator*(Vec v) {
	return Vec(
		A * v.x + B * v.y + C,
		D * v.x + E * v.y + F
	);
}

/* Thanks to
 * https://www.wolframalpha.com/input/?i=inverse+of+%5B%5Bx0,x1,x2%5D,%5Bx3,x4,x5%5D,%5B0,0,1%5D%5D
 */
Transform Transform::inverse() {
	float denom = A * E - B * D;
	if(denom == 0) {
		/* TODO: Figure out if this is actually possible,
		 * and, if so, figure out a better solution than
		 * silently ignoring it.
		 */
		return Transform();
	}
	/* Note that the third argument has the multiplications reversed
	 * from Wolfram Alpha so that we can divide by denom rather than
	 * -denom */
	return Transform(E /  denom, B / -denom, (B * F - C * E) / denom,
	                 D / -denom, A /  denom, (C * D - A * F) / denom);
}