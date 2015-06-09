//
// Created by dbaumeister on 19.05.15.
//

#ifndef GRAPHICS_GRID_H
#define GRAPHICS_GRID_H


#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../General/Definitions.h"
#include "../VectorMath/Vector.h"

class RealGrid {
public:
    RealGrid(int dimX, int dimY, int dimZ = 1) : dimX(dimX), dimY(dimY), dimZ(dimZ) {
        values = (Real*) calloc((size_t) dimZ * (size_t)dimY * (size_t)dimX, sizeof(Real));
        clear();
    }

    explicit RealGrid(RealGrid& grid) = delete;

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

    Real& at(int x, int y, int z){
        return values[offset(x, y, z)];
    }

    Real& at(int x, int y){
        return values[offset(x, y, 0)];
    }

    Real& operator()(int x, int y, int z){
        return values[offset(x, y, z)];
    }

    Real& operator()(int x, int y){
        return values[offset(x, y, 0)];
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

    void clear(){
        for(int k = 0; k < dimZ; ++k){
            for(int j = 0; j < dimY; ++j){
                for(int i = 0; i < dimX; ++i){
                    values[offset(i, j, k)] = 0;
                }
            }
        }
    }


    void set(RealGrid& grid){
        for(int k = 0; k < getDimZ(); ++k){
            for (int j = 0; j < getDimY(); ++j) {
                for (int i = 0; i < getDimX(); ++i) {
                    values[offset(i, j, k)] = grid(i, j, k);
                }
            }
        }
    }


private:
    int dimX, dimY, dimZ;
    Real* values;

    int offset(int x, int y, int z) {
        assert(x >= 0);
        assert(y >= 0);
        assert(z >= 0);
        assert(x < dimX);
        assert(y < dimY);
        assert(z < dimZ);
        return x + y * dimX + z * dimX * dimY;
    }
};



class VectorGrid {
public:
    VectorGrid(int dimX, int dimY, int dimZ = 1) : dimX(dimX), dimY(dimY), dimZ(dimZ) {
        values = (Vector3D*) calloc((size_t)dimZ * (size_t)dimY * (size_t)dimX, sizeof(Vector3D));
        clear();
    }


    explicit VectorGrid(VectorGrid& grid) = delete;

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

    Vector3D& operator()(int x, int y, int z){
        return values[offset(x, y, z)];
    }

    Vector3D& operator()(int x, int y){
        return values[offset(x, y, 0)];
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

    void clear(){
        for(int k = 0; k < dimZ; ++k){
            for(int j = 0; j < dimY; ++j){
                for(int i = 0; i < dimX; ++i){
                    values[offset(i, j, k)].x = 0;
                    values[offset(i, j, k)].y = 0;
                    values[offset(i, j, k)].z = 0;
                }
            }
        }
    }


    void set(VectorGrid& grid){
        for(int k = 0; k < getDimZ(); ++k){
            for (int j = 0; j < getDimY(); ++j) {
                for (int i = 0; i < getDimX(); ++i) {
                    values[offset(i, j, k)] = grid(i, j, k);
                }
            }
        }
    }

private:
    int dimX, dimY, dimZ;
    Vector3D* values;

    int offset(int x, int y, int z) {
        assert(x >= 0);
        assert(y >= 0);
        assert(z >= 0);
        assert(x < dimX);
        assert(y < dimY);
        assert(z < dimZ);
        return x + y * dimX + z * dimX * dimY;
    }
};


std::ostream& operator <<(std::ostream& stream, RealGrid& grid);

std::ostream& operator <<(std::ostream& stream, VectorGrid& grid);


#endif //GRAPHICS_GRID_H
