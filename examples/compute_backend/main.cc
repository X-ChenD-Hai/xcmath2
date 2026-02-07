// #define ENABLE_COL_MAJOR_MAT
#include <cassert>
#include <iostream>
#include <ostream>
#include <xcmath/mat.hpp>
#include <xcmath/serialize.hpp>
#include <xcmath/vec.hpp>

using namespace xcmath;

auto get_m() {
    mat<float, 2, 3, false> a{};

    a[0][0] = 1;
    a[0][1] = 2;
    a[0][2] = 3;
    a[1][0] = 4;
    a[1][1] = 5;
    a[1][2] = 6;
    return a;
}

int main() {
    auto v = get_m();
    xcmath::vec3<float> v1{1, 1, 3};
    xcmath::vec3<float> v2{1, 2, 3};
    auto m = xcmath::mat3<float>{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    std::cout << v2.cross(v1) << std::endl;
    std::cout << v2.cross(v1).module() << std::endl;
    std::cout << v2.cross(v1).sign() << std::endl;
    std::cout << m.trace() << std::endl;

    return 0;
}