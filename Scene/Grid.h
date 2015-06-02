//
// Created by dbaumeister on 19.05.15.
//

#ifndef GRAPHICS_GRID_H
#define GRAPHICS_GRID_H


#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include "../VectorMath/VectorMath.h"

class RealGrid {
public:
    RealGrid(int dimX, int dimY, int dimZ = 1) : dimX(dimX), dimY(dimY), dimZ(dimZ) {
        values = (Real*) calloc(dimZ * dimY * dimX, sizeof(Real));
    }

    ~RealGrid() {
        free(values);
    }

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

    Real& operator()(int x, int y, int z = 0){
        return values[offset(x, y, z)];
    }

    int getDimX(){
        return dimX;
    }
    int getDimY(){
        return dimY;
    }
    int getDimZ(){
        return dimZ;
    }

private:
    int dimX, dimY, dimZ;
    Real* values;

    int offset(int x, int y, int z) {
        return x + y * dimX + z * dimX * dimY;
    }
};



template <class T>
class VectorGrid {
public:
    VectorGrid(int dimX, int dimY, int dimZ = 1) : dimX(dimX), dimY(dimY), dimZ(dimZ) {
        values = (Vector3D<T>*) calloc(dimZ * dimY * dimX, sizeof(Vector3D<T>));
    }

    ~VectorGrid() {
        free(values);
    }


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

    Vector3D<T>& operator()(int x, int y, int z = 0){
        return values[offset(x, y, z)];
    }


    int getDimX(){
        return dimX;
    }
    int getDimY(){
        return dimY;
    }
    int getDimZ(){
        return dimZ;
    }

private:
    int dimX, dimY, dimZ;
    Vector3D<T>* values;

    int offset(int x, int y, int z) {
        return x + y * dimX + z * dimX * dimY;
    }
};


std::ostream& operator <<(std::ostream& stream, RealGrid& grid);

template <class T>
std::ostream& operator <<(std::ostream& stream, VectorGrid<T>& grid);

#endif //GRAPHICS_GRID_H
