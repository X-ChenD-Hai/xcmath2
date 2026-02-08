#include <type_traits>
#define ENABLE_COL_MAJOR_MAT
#include <cassert>
#include <iostream>
#include <ostream>
#include <xcmath/mat.hpp>
#include <xcmath/serialize.hpp>
#include <xcmath/vec.hpp>

#include "xcmath/methods.hpp"

using namespace xcmath;

auto get_m() {
    mat3f a{};

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

    auto m1 = get_m();

    using vec3f_view = vec_view<float, 3, 3>;
    vec3f_view av = m1[0];
    static_assert(std::is_base_of_v<all_method<EmptyBase, vec3f>, vec3f>, "");
    // static_assert(
    //     std::is_base_of_v<size_method<EmptyBase, vec3f_view>, vec3f_view>,
    //     "");
    // auto m21 = av.move();
    auto m22 = av.all();
    // av.module();
    auto data = m1[0].cross(m1[1]).cross(m[2].clone());
    std::cout << data << std::endl;
    std::cout << vec3f::zero() << std::endl;

    return 0;
}