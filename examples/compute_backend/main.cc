// #define ENABLE_COL_MAJOR_MAT
#include <cassert>
#include <iostream>
#include <ostream>
#include <xcmath/mat.hpp>
#include <xcmath/serialize.hpp>
#include <xcmath/vec.hpp>

using namespace xcmath;

auto get_m() {
    mat<int, 2, 3, true> a{};

    a[0, 0] = 1;
    a[0, 1] = 2;
    a[0, 2] = 3;
    a[1, 0] = 4;
    a[1, 1] = 5;
    a[1, 2] = 6;
    return a;
}
class EmptyBase {};

template <class Base>
struct D : Base {
    void d();
};
template <class Base>
struct C : Base {
    void c();
};

int main() {
    xcmath::mat3f v{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    xcmath::vec3<float> v2{1, 2, 3};

    std::cout << v2.normalize() << std::endl;

    std::cout << "size: " << sizeof(vec3<float>) << std::endl;
    std::cout << "size: " << sizeof(mat3<float>) << std::endl;

    auto start = &(v[0][0]);
    assert((start == &v[0, 0]));
    std::cout << (&(v[0, 0]) - start) << " = " << v[0, 0] << std::endl;
    std::cout << (&(v[0, 1]) - start) << " = " << v[0, 1] << std::endl;
    std::cout << (&(v[0, 2]) - start) << " = " << v[0, 2] << std::endl;
    std::cout << (&(v[1, 0]) - start) << " = " << v[1, 0] << std::endl;
    std::cout << (&(v[1, 1]) - start) << " = " << v[1, 1] << std::endl;
    std::cout << (&(v[1, 2]) - start) << " = " << v[1, 2] << std::endl;
    std::cout << (&(v[2, 0]) - start) << " = " << v[2, 0] << std::endl;
    std::cout << (&(v[2, 1]) - start) << " = " << v[2, 1] << std::endl;
    std::cout << (&(v[2, 2]) - start) << " = " << v[2, 2] << std::endl;

    // std::cout << mat3<float>::zero() << std::endl;
    std::cout << v << std::endl;

    return 0;
}