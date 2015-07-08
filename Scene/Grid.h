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
#include "../General/Definitions.h"
#include "../VectorMath/Vector.h"

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
    virtual ValueType getDefaultValue() = 0;


    ValueType& operator()(int x, int y, int z){
        return values[offset(x, y, z)];
    }

    ValueType& operator()(int x, int y){
        return values[offset(x, y, 0)];
    }

    inline int getDimX(){
        return dimX;
    }
    inline int getDimY(){
        return dimY;
    }
    inline int getDimZ(){
        return dimZ;
    }


    ValueType getInterpolated(Real ri, Real rj, Real rk){

        Real di = ri - (int)ri;
        Real dj = rj - (int)rj;
        Real dk = rk - (int)rk;


        return (1 - di) * (1 - dj) * (1 - dk) * values[offset(ri, rj, rk)]
                + (di) * (1 - dj) * (1 - dk) * values[offset(ri + 1, rj, rk)]
                  + (1 - di) * (dj) * (1 - dk) * values[offset(ri, rj + 1, rk)]
                    + (di) * (dj) * (1 - dk) * values[offset(ri + 1, rj + 1, rk)]
                      + (1 - di) * (1 - dj) * (dk) * values[offset(ri, rj, rk + 1)]
                        + (di) * (1 - dj) * (dk) * values[offset(ri + 1, rj, rk + 1)]
                          + (1 - di) * (dj) * (dk) * values[offset(ri, rj + 1, rk + 1)]
                            + (di) * (dj) * (dk) * values[offset(ri + 1, rj + 1, rk + 1)];
    }

    ValueType getInterpolated(Real ri, Real rj){
        Real di = ri - (int)ri;
        Real dj = rj - (int)rj;


        return (1 - di) * (1 - dj) * values[offset(ri, rj, 0)]
               + (di) * (1 - dj) * values[offset(ri + 1, rj, 0)]
               + (1 - di) * (dj) * values[offset(ri, rj + 1, 0)]
               + (di) * (dj) * values[offset(ri + 1, rj + 1, 0)];
    }

    void swap(Grid<ValueType>& grid){
        std::swap(getRawPointerReference(), grid.getRawPointerReference());
    }

    ValueType*& getRawPointerReference(){
        return values;
    }

protected:
    int dimX, dimY, dimZ;
    ValueType* values;

    inline int offset(int x, int y, int z) {
        return x + y * dimX + z * dimX * dimY;
    }
};


class RealGrid : public Grid<Real>{
public:
    RealGrid(int dimX, int dimY, int dimZ = 1) : Grid<Real>(dimX, dimY, dimZ) {}

    explicit RealGrid(RealGrid& grid) = delete;

    std::string toString() {
        std::stringstream out;

        for(int k = 0; k < dimZ; ++k){
            out << "Slice " << k << ":\n";
            for(int j = 0; j < dimY; ++j){
                out << "[ ";
                for(int i = 0; i < dimX; ++i){
                    out << values[offset(i, j, k)] << " ";
                }
                out << "]\n";
            }
            out << "\n";
        }
        return out.str();
    }

    Real getDefaultValue() {
        return 0;
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


    Vector3D getDefaultValue() {
        return Vector3D(0, 0, 0);
    }
};

template <class ValueType>
std::ostream& operator <<(std::ostream& stream, Grid<ValueType>& grid){
    stream << grid.toString();
    return stream;
}

#endif //GRAPHICS_GRID_H
