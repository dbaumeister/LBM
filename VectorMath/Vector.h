//
// Created by dbaumeister on 19.05.15.
//

#ifndef GRAPHICS_VECTOR_H
#define GRAPHICS_VECTOR_H

#include <sstream>
#include <string>
#include <stdint.h>
#include <malloc.h>
#include <math.h>


#include "../General/Definitions.h"

class Vector3D{
public:
    Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(v.z) {}

    Vector3D(Real x = 0, Real y = 0, Real z = 0) : x(x), y(y), z(z){}

    Real x, y, z;

    std::string toString(){
        std::stringstream out;
        out << "[ " << x << "; " << y << "; " << z << "; ]";
        return out.str();
    }


    void operator *=(Real v){
        x *= v;
        y *= v;
        z *= v;
    }

    void operator +=(Vector3D other){
        x += other.x;
        y += other.y;
        z += other.z;
    }

    void operator -=(Vector3D other){
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    Real dot(Vector3D& other){
        return x * other.x + y * other.y + z * other.z;
    }

    Real norm(){
        return sqrt(x * x + y * y + z * z);
    }
};

std::ostream& operator <<(std::ostream& stream, Vector3D vector);


Vector3D operator +(Vector3D left, Vector3D right);

Vector3D operator -(Vector3D left, Vector3D right);

/*
 * Cross product
 */
Vector3D operator %(Vector3D left, Vector3D right);


Real operator *(Vector3D left, Vector3D right);

Vector3D operator *(Real v, Vector3D right);

Vector3D operator *(Vector3D left, Real v);


#endif //GRAPHICS_VECTOR_H
