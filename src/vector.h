#ifndef VEC_H
#define VEC_H

class Vec {
public:
    float x, y;
    
    Vec();
    Vec(float);
    Vec(float,float);
    
    Vec operator+(Vec) const;
    Vec operator-(Vec) const;
    void operator+=(Vec);
    void operator-=(Vec);
    
    Vec operator*(float) const;
    Vec operator/(float) const;
    void operator*=(float);
    void operator/=(float);
    
    float dot(Vec) const;
    
    float len() const;
    float lensqr() const;
    
    void normalize();
    Vec normalized() const;
    
    void scale(float);
    Vec scaled(float) const;
    
    Vec ortho() const;
    
    Vec bisector(Vec) const;
};

#endif