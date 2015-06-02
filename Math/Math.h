//
// Created by dbaumeister on 21.05.15.
//

#ifndef GRAPHICS_GMATH_H
#define GRAPHICS_GMATH_H

#include "Vector.h"

#define Real float


template <class T>
Vector3D<T> operator +(Vector3D<T> left, Vector3D<T> right){
    Vector3D<T> out(left);
    out += right;
    return out;
}

template <class T>
Vector3D<T> operator -(Vector3D<T> left, Vector3D<T> right){
    Vector3D<T> out(left);
    out -= right;
    return out;
}

/*
 * Cross product
 */
template <class T>
Vector3D<T> operator %(Vector3D<T> left, Vector3D<T> right){
    Vector3D<T> out(
            left.y * right.z - left.z * right.y,
            left.z * right.x - left.x * right.z,
            left.x * right.y - left.y * right.x
    );
    return out;
}


template <class T>
T operator *(Vector3D<T> left, Vector3D<T> right){
    Vector3D<T> out(left);
    return out.dot(right);
}

Vector3D<Real> operator *(Real v, Vector3D<Real> right){
    Vector3D<Real> out(right);
    out *= v;
    return out;
}

Vector3D<Real> operator *(Vector3D<Real> left, Real v){
    Vector3D<Real> out(left);
    out *= v;
    return out;
}

#endif //GRAPHICS_GMATH_H
