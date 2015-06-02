//
// Created by dbaumeister on 02.06.15.
//

#include "Vector.h"

template <class T>
std::ostream& operator <<(std::ostream& stream, Vector3D<T> vector){
    stream << vector.toString();
    return stream;
}