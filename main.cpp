#include <iostream>
#include "Math/Grid.h"
#include "Math/Vector.h"


int main() {

    Vector3D<Real> a;
    Vector3D<Real> b;

    b.x = 4;

    a.x = 1;
    a.y = 2;
    a.z = 3;

    RealGrid grid(2,2);
    std::cout << (3 * a * b * 4 * b * 4).toString() << std::endl;

    return 0;
}