//
// Created by dbaumeister on 02.06.15.
//

#include "Grid.h"

std::ostream& operator <<(std::ostream& stream, RealGrid& grid){
    stream << grid.toString();
    return stream;
}

template <class T>
std::ostream& operator <<(std::ostream& stream, VectorGrid<T>& grid){
    stream << grid.toString();
    return stream;
}