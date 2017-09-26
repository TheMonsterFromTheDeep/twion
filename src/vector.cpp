#include "vector.h"

#include <cmath>

Vec::Vec() : x(0), y(0) { }
Vec::Vec(float dupli) : x(dupli), y(dupli) { }
Vec::Vec(float x_, float y_) : x(x_), y(y_) { }

Vec Vec::operator+(Vec v) const { return Vec(x + v.x, y + v.y); }
Vec Vec::operator-(Vec v) const { return Vec(x - v.x, y - v.y); }

Vec Vec::operator*(float s) const { return Vec(x * s, y * s); }
Vec Vec::operator/(float s) const { return Vec(x / s, y / s); }

void Vec::operator*=(float s) { x *= s; y *= s; }
void Vec::operator/=(float s) { x /= s; y /= s; }

float Vec::dot(Vec v) const { return x * v.x + y * v.y; }

float Vec::lensqr() const { return x * x + y * y; }

float Vec::len() const { return sqrt(lensqr()); }

void Vec::normalize() { operator/=(len()); }
Vec Vec::normalized() const { return operator/(len()); }

void Vec::scale(float s) { operator*=(s / len()); }
Vec Vec::scaled(float s) const { return operator*(s / len()); }

Vec Vec::ortho() const { return Vec(-y, x); }

Vec Vec::bisector(Vec v) const {
	Vec bisec = operator*(v.len()) + v * len();
	if(bisec.lensqr() == 0) bisec = ortho();
	return bisec;
}