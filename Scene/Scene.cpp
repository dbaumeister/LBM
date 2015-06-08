//
// Created by dbaumeister on 03.06.15.
//

#include <assert.h>
#include "Scene.h"

void applyDensityBlock(Scene2D &scene, int x, int y, int width, int height, Real val) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x + width <= scene.getDimX());
    assert(y + height <= scene.getDimY());

    for(int i = x; i < x + width; ++i){
        for(int j = y; j < y + height; ++j){
            scene.density(i, j) = val;
        }
    }
}

void applyVelocityBlock(Scene2D &scene, int x, int y, int width, int height, Vector3D val) {
    assert(x >= 0);
    assert(y >= 0);
    assert(x + width <= scene.getDimX());
    assert(y + height <= scene.getDimY());

    for(int i = x; i < x + width; ++i){
        for(int j = y; j < y + height; ++j){
            scene.vel(i, j) = val;
        }
    }
}
