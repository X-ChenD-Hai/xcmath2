#include <iostream>
#define ENABLE_COL_MAJOR_MAT
#include <xcmath/alias.hpp>
#include <xcmath/mat.hpp>
#include <xcmath/mat_factories.hpp>
#include <xcmath/serialize.hpp>
#include <xcmath/transform_methods.hpp>
#include <xcmath/vec.hpp>

using namespace xcmath;

int main() {
    // auto v = get_m();
    xcmath::vec3<float> v1{1, 1, 3};
    xcmath::vec3<float> v2{1, 2, 3};
    auto m = xcmath::mat3<float>{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    std::cout << v2.cross(v1) << std::endl;
    std::cout << v2.cross(v1).module() << std::endl;
    std::cout << v2.cross(v1).sign() << std::endl;
    std::cout << m.trace() << std::endl;

    const auto mmm = mat3f::unit();
    auto vv = mmm[0].clone();


    auto a = m[0].clone();

    std::cout << vec3f::zero() << std::endl;

    auto m44 = mat4f::unit();
    auto m33 = mat3f::unit();
    auto m33_2d_ = mat3f::unit();
    auto m22_ = mat2f::unit();
    auto vec4_ = vec4f{1, 0, 0, 1};
    auto vec3_ = vec3f{1, 0, 0};
    auto vec3_2d_ = vec3f{1, 0, 0};
    auto vec2_ = vec2f{1, 0};
    auto angle = 60.f;
    std::cout << m44.scale(2.f)
                         .rotate(angle, vec3f{0, 0, 1})
                         .translate(vec3f{1, 2, 1}) *
                     vec4_
              << std::endl;
    std::cout << m33.rotate(angle, vec3f{0, 0, 1}) * vec3_ << std::endl;
    std::cout << m33_2d_.rotate(angle).scale(2.f) * vec3_2d_ << std::endl;
    std::cout << m33_2d_[1, 2] << std::endl;
    std::cout << vec3_ << m33[1] << std::endl;
    std::cout << (vec3_ == m33[1]).all() << std::endl;
    std::cout << vec3_ << m33[1] << std::endl;
    std::cout << (m33[1] == m33[1]).all() << std::endl;
    std::cout << m33 << std::endl;
    std::cout << m33_2d_.translate(vec2f{1, 2}) * m33 << std::endl;
    std::cout << mat4f::look_at(vec3f{0, 0, 0}, vec3f{0, 0, 1}, vec3f{0, 1, 0})
              << std::endl;

    return 0;
}