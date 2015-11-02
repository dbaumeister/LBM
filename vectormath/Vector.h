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


#include "../general/Definitions.h"

class Vector3D{
public:
    Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(v.z) {}

    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z){}

    double x, y, z;

    std::string toString(){
        std::stringstream out;
        out << "[ " << x << "; " << y << "; " << z << "; ]";
        return out.str();
    }


    void operator *=(double v){
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

    double dot(Vector3D& other){
        return x * other.x + y * other.y + z * other.z;
    }
};

std::ostream& operator <<(std::ostream& stream, Vector3D vector);


Vector3D operator +(Vector3D left, Vector3D right);

Vector3D operator -(Vector3D left, Vector3D right);

/*
 * Cross product
 */
Vector3D operator %(Vector3D left, Vector3D right);


double operator *(Vector3D left, Vector3D right);

Vector3D operator *(double v, Vector3D right);

Vector3D operator *(Vector3D left, double v);


#endif //GRAPHICS_VECTOR_H
