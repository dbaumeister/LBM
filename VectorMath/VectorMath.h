//
// Created by dbaumeister on 21.05.15.
//

#ifndef GRAPHICS_GMATH_H
#define GRAPHICS_GMATH_H

#include "Vector.h"

#define Real float


template <class T>
Vector3D<T> operator +(Vector3D<T> left, Vector3D<T> right);

template <class T>
Vector3D<T> operator -(Vector3D<T> left, Vector3D<T> right);

/*
 * Cross product
 */
template <class T>
Vector3D<T> operator %(Vector3D<T> left, Vector3D<T> right);


template <class T>
T operator *(Vector3D<T> left, Vector3D<T> right);

Vector3D<Real> operator *(Real v, Vector3D<Real> right);

Vector3D<Real> operator *(Vector3D<Real> left, Real v);

#endif //GRAPHICS_GMATH_H
