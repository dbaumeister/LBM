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

template<class T>
class Vector3D{
public:
    Vector3D(const Vector3D<T>& v) : x(v.x), y(v.y), z(v.z) {}

    Vector3D(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z){}

    T x, y, z;

    std::string toString(){
        std::stringstream out;
        out << "[ " << x << "; " << y << "; " << z << "; ]";
        return out.str();
    }


    void operator *=(T v){
        x *= v;
        y *= v;
        z *= v;
    }

    void operator +=(Vector3D<T>& other){
        x += other.x;
        y += other.y;
        z += other.z;
    }

    void operator -=(Vector3D<T>& other){
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    T dot(Vector3D<T>& other){
        return x * other.x + y * other.y + z * other.z;
    }

    T norm(){
        return sqrt(x * x + y * y + z * z);
    }
};

template <class T>
std::ostream& operator <<(std::ostream& stream, Vector3D<T> vector);

#endif //GRAPHICS_VECTOR_H
