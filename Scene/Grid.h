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

    void swap(Grid<ValueType>& grid){
        ValueType* vs = grid.getRawPointer();
        grid.setRawPointer(values);
        values = vs;
    }

    ValueType* getRawPointer(){
        return values;
    }

    void setRawPointer(ValueType* vs){
        values = vs;
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

    /*
     * returns the next smalles index to i, which can be used for interpolation
     */
    inline int getIndex(Real i){
        return (int)(i - 0.5f);
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


    /*
     * returns the next smalles index to i, which can be used for interpolation
     */
    int getIndex(Real i){
        return (int) i;
    }
};

template <class ValueType>
std::ostream& operator <<(std::ostream& stream, Grid<ValueType>& grid){
    stream << grid.toString();
    return stream;
}

#endif //GRAPHICS_GRID_H
