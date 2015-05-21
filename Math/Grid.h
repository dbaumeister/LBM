//
// Created by dbaumeister on 19.05.15.
//

#ifndef GRAPHICS_GRID_H
#define GRAPHICS_GRID_H


#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include "Math.h"
#include "Vector.h"

class RealGrid {
public:
    RealGrid(uint64_t dimX, uint64_t dimY, uint64_t dimZ = 1) : dimX(dimX), dimY(dimY), dimZ(dimZ) {
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
    /*
     * Prints the grid at z = slice
     */
    void print(uint64_t slice){

    }

    Real& operator()(uint64_t x, uint64_t y, uint64_t z = 0){
        return values[offset(x, y, z)];
    }

    uint64_t getDimX(){
        return dimX;
    }
    uint64_t getDimY(){
        return dimY;
    }
    uint64_t getDimZ(){
        return dimZ;
    }

private:
    uint64_t dimX, dimY, dimZ;
    Real* values;

    uint64_t offset(uint64_t x, uint64_t y, uint64_t z) {
        return x + y * dimX + z * dimX * dimY;
    }
};



template <class T>
class VectorGrid {
public:
    VectorGrid(uint64_t dimX, uint64_t dimY, uint64_t dimZ = 1) : dimX(dimX), dimY(dimY), dimZ(dimZ) {
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

    /*
     * Prints the grid at z = slice
     */
    void print(uint64_t slice){

    }

    Vector3D<T>& operator()(uint64_t x, uint64_t y, uint64_t z = 0){
        return values[offset(x, y, z)];
    }


    uint64_t getDimX(){
        return dimX;
    }
    uint64_t getDimY(){
        return dimY;
    }
    uint64_t getDimZ(){
        return dimZ;
    }

private:
    uint64_t dimX, dimY, dimZ;
    Vector3D<T>* values;

    uint64_t offset(uint64_t x, uint64_t y, uint64_t z) {
        return x + y * dimX + z * dimX * dimY;
    }
};


#endif //GRAPHICS_GRID_H
