//
// Created by dbaumeister on 19.05.15.
//

#ifndef GRAPHICS_GRID_H
#define GRAPHICS_GRID_H


#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "../general/Definitions.h"
#include "../vectormath/Vector.h"

template <class ValueType>
class Grid {
public:
    Grid(int dimX, int dimY, int dimZ = 1) : dimX(dimX), dimY(dimY), dimZ(dimZ) {
        values = (ValueType*) calloc((size_t) dimZ * (size_t)dimY * (size_t)dimX, sizeof(ValueType));
    }

    explicit Grid(Grid& grid) = delete;

    ~Grid() {
        free(values);
    }

    virtual std::string toString() = 0;

    ValueType get(int x, int y) const {
        return values[offset(x, y, 0)];
    }

    ValueType& operator()(int x, int y, int z){
        return values[offset(x, y, z)];
    }

    ValueType& operator()(int x, int y){
        return values[offset(x, y, 0)];
    }

    inline int getDimX() const {
        return dimX;
    }
    inline int getDimY() const {
        return dimY;
    }

    ValueType*& getRawPointerReference(){
        return values;
    }

protected:
    int dimX, dimY, dimZ;
    ValueType* values;

    inline int offset(int x, int y, int z) const {
        return x + y * dimX + z * dimX * dimY;
    }
};


class VectorGrid : public Grid<Vector3D> {
public:
    VectorGrid(int dimX, int dimY, int dimZ = 1) : Grid<Vector3D>(dimX, dimY, dimZ) {}

    explicit VectorGrid(VectorGrid& grid) = delete;


    std::string toString() {
        std::stringstream out;

        for(int k = 0; k < dimZ; ++k){
            out << "Slice " << k << ":\n";
            for(int j = 0; j < dimY; ++j){
                out << "[ ";
                for(int i = 0; i < dimX; ++i){
                    out << values[offset(i, j, k)].toString() << " ";
                }
                out << "]\n";
            }
            out << "\n";
        }
        return out.str();
    }
};

template <class ValueType>
std::ostream& operator <<(std::ostream& stream, Grid<ValueType>& grid){
    stream << grid.toString();
    return stream;
}

#endif //GRAPHICS_GRID_H
