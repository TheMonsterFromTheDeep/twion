#include "vector.h"

#include <cmath>

Vec::Vec() : x(0), y(0) { }
Vec::Vec(float dupli) : x(dupli), y(dupli) { }
Vec::Vec(float x_, float y_) : x(x_), y(y_) { }

Vec Vec::operator+(Vec v) const { return Vec(x + v.x, y + v.y); }
Vec Vec::operator-(Vec v) const { return Vec(x - v.x, y - v.y); }

void Vec::operator+=(Vec v) { x += v.x; y += v.y; }
void Vec::operator-=(Vec v) { x -= v.x; y -= v.y; }

Vec Vec::operator*(float s) const { return Vec(x * s, y * s); }
Vec Vec::operator/(float s) const { return Vec(x / s, y / s); }

void Vec::operator*=(float s) { x *= s; y *= s; }
void Vec::operator/=(float s) { x /= s; y /= s; }

float Vec::dot(Vec v) const { return x * v.x + y * v.y; }

float Vec::component(Vec v) const {
    return dot(v) / v.len();
}

Vec Vec::project(Vec v) const {
    return (v * dot(v)) / v.lensqr();
}

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

#define DEG_TO_RAD 0.01745329251

Vec Vec::rotate(float deg, Vec center) {
    Vec delta = operator-(center);
    
    deg *= DEG_TO_RAD;
    
    Vec rotated;
    rotated.x = delta.x * ::cos(deg) - delta.y * ::sin(deg);
    rotated.y = delta.x * ::sin(deg) + delta.y * ::cos(deg);
    
    return center + rotated;
}

Vec Vec::rotate(Vec direction, Vec center) {
    Vec delta = operator-(center);
    
    float cos_val = direction.x / direction.len();
    float sin_val = direction.y / direction.len();
    
    Vec rotated;
    rotated.x = delta.x * cos_val - delta.y * sin_val;
    rotated.y = delta.x * sin_val + delta.y * cos_val;
    
    return center + rotated;
}

Vec Vec::rotate_inverse(Vec direction, Vec center) {
    Vec delta = operator-(center);
    
    float cos_val = direction.x / direction.len();
    float sin_val = -direction.y / direction.len();
    
    Vec rotated;
    rotated.x = delta.x * cos_val - delta.y * sin_val;
    rotated.y = delta.x * sin_val + delta.y * cos_val;
    
    return center + rotated;
}

Vec Vec::scale(float amount, Vec center) {
    Vec delta = operator-(center);
    delta *= amount;
    return center + delta;
}

Vec Vec::scale(Vec amount, Vec center) {
    Vec delta = operator-(center);
    delta.x *= amount.x;
    delta.y *= amount.y;
    return center + delta;
}

float Vec::angle(Vec other) {
    Vec delta = other.operator-(*this);
    
    return atan2(delta.y, delta.x);
}

float Vec::cross_len(Vec other) {
    return (x * other.y) - (other.x * y);
}

float Vec::cos(Vec other) {
    return dot(other) / (len() * other.len());
}

float Vec::sin(Vec other) {
    return cross_len(other) / (len() * other.len());
}