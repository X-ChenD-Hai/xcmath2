#pragma once
namespace xcmath {
template <typename Base, typename Derived, typename mata, size_t size_>
struct point_accesser : Base {};
template <typename Base, typename Derived, typename meta>
struct point_accesser<Base, Derived, meta, 1> : Base {
    inline constexpr auto& x() noexcept {
        return static_cast<Derived*>(this)->operator[](0);
    }
    inline constexpr const auto& x() const noexcept {
        return static_cast<Derived*>(this)->operator[](0);
    }
};
template <typename Base, typename Derived, typename meta>
struct point_accesser<Base, Derived, meta, 2>
    : point_accesser<Base, Derived, meta, 1> {
    inline constexpr auto& y() noexcept {
        return static_cast<Derived*>(this)->operator[](1);
    }
    inline constexpr const auto& y() const noexcept {
        return static_cast<Derived*>(this)->operator[](1);
    }
};
template <typename Base, typename Derived, typename meta>
struct point_accesser<Base, Derived, meta, 3>
    : point_accesser<Base, Derived, meta, 2> {
    inline constexpr auto& z() noexcept {
        return static_cast<Derived*>(this)->operator[](2);
    }
    inline constexpr const auto& z() const noexcept {
        return static_cast<Derived*>(this)->operator[](2);
    }
};
template <typename Base, typename Derived, typename meta>
struct point_accesser<Base, Derived, meta, 4>
    : point_accesser<Base, Derived, meta, 3> {
    inline constexpr auto& w() noexcept {
        return static_cast<Derived*>(this)->operator[](3);
    }
    inline constexpr const auto& w() const noexcept {
        return static_cast<Derived*>(this)->operator[](3);
    }
};

template <size_t size_>
struct point_accesser_sized {
    template <typename Base, typename Derived, typename meta>
    using type = point_accesser<Base, Derived, meta, size_>;
};

}  // namespace xcmath