#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vector.h"

/* Represents a 2D affine transform with the use of matrices.
 *
 * 2D affine transformations generally use 3D matrices, because
 * otherwise matrix multiplication is not well-defined.
 *
 * But this is silly, because the last row of our matrix, if it
 * is 3x3, will always be [0 0 1]. This is because all the matrices
 * we construct will have this as the last row, and when we multiply
 * them together, they will still have this as the last row.
 * (see https://www.wolframalpha.com/input/?i=%5B%5Bx0,x1,x2%5D,%5Bx3,x4,x5%5D,%5B0,0,1%5D%5D+*+%5B%5By0,y1,y2%5D,%5By3,y4,y5%5D,%5B0,0,1%5D%5D )
 *
 * So, in essence, storing these values is a waste of 12 bytes per
 * Transform object, and therefore we won't do that. Instead, we just
 * pretend that we do have a third row filled with [0 0 1], and take
 * that into account during multiplication.
 *
 * It also happens that this reduces the number of operations in
 * the matrix multiplication by a rather significant amount.
 */
class Transform {
public:
	float A, B, C, D, E, F;
	
	Transform();
	Transform(const Transform&);
	
	Transform(float,float,
	          float,float);
	Transform(float,float,float,
	          float,float,float);
	
	static Transform rotation(float);
	static Transform rotation(float, float, float);
	static Transform rotation(float, Vec);
	static Transform rotation(Vec);
	static Transform rotation(Vec, float, float);
	static Transform rotation(Vec, Vec);
	static Transform translation(float,float);
	static Transform translation(Vec);
	static Transform scale(float);
	static Transform scale(float,float);
	static Transform scale(Vec);
	static Transform scale(float, float, float);
	static Transform scale(float, Vec);
	static Transform scale(float, float, float, float);
	static Transform scale(float, float, Vec);
	static Transform scale(Vec, float, float);
	static Transform scale(Vec, Vec);
	
	Transform operator*(const Transform&) const;
	Vec operator*(Vec) const;
	
	Transform inverse();
};

#endif