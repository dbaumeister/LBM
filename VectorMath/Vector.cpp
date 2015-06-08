//
// Created by dbaumeister on 02.06.15.
//

#include "Vector.h"

std::ostream& operator <<(std::ostream& stream, Vector3D vector){
    stream << vector.toString();
    return stream;
}

Vector3D operator +(Vector3D left, Vector3D right){
    Vector3D out(left);
    out += right;
    return out;
}

Vector3D operator -(Vector3D left, Vector3D right){
    Vector3D out(left);
    out -= right;
    return out;
}

/*
 * Cross product
 */
Vector3D operator %(Vector3D left, Vector3D right){
    Vector3D out(
            left.y * right.z - left.z * right.y,
            left.z * right.x - left.x * right.z,
            left.x * right.y - left.y * right.x
    );
    return out;
}


Real operator *(Vector3D left, Vector3D right){
    Vector3D out(left);
    return out.dot(right);
}

Vector3D operator *(Real v, Vector3D right){
    Vector3D out(right);
    out *= v;
    return out;
}

Vector3D operator *(Vector3D left, Real v){
    Vector3D out(left);
    out *= v;
    return out;
}