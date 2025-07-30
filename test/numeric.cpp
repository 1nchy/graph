#include "test.hpp"

#include <type_traits>
#include <limits>

/**
 * @return whether `_s > _a + _b`, no overflow
 */
template <typename _Tp> auto greater_than(_Tp _s, _Tp _a, _Tp _b) -> bool {
    static_assert(std::is_arithmetic<_Tp>::value);
    if constexpr (std::is_floating_point<_Tp>::value) {
        return _s > _a + _b;
    }
    if constexpr (std::is_signed<_Tp>::value) {
        if (_b > 0) {
            if (_a > std::numeric_limits<_Tp>::max() - _b) {
                return false;
            }
        }
        else if (_b < 0) {
            if (_a < std::numeric_limits<_Tp>::min() - _b) {
                return true;
            }
        }
        return _s > _a + _b;
    }
    if constexpr (std::is_unsigned<_Tp>::value) {
        if (_a > std::numeric_limits<_Tp>::max() - _b) {
            return false;
        }
        return _s > _a + _b;
    }
    return _s > _a + _b;
};

template <typename _Tp> auto MAX() -> _Tp {
    static_assert(std::is_arithmetic<_Tp>::value);
    return std::numeric_limits<_Tp>::max();
}
template <typename _Tp> auto MIN() -> _Tp {
    static_assert(std::is_arithmetic<_Tp>::value);
    return std::numeric_limits<_Tp>::min();
}

ICY_CASE("float") {

}
ICY_CASE("signed") {
    ICY_SUBCASE("MAX, 0, 0") {
        EXPECT_TRUE(greater_than(MAX<signed>(), 0, 0));
        EXPECT_TRUE(greater_than(MAX<signed>(), 0, 3));
        EXPECT_TRUE(greater_than(MAX<signed>(), 0, -3));
        EXPECT_TRUE(greater_than(MAX<signed>(), 3, 0));
        EXPECT_TRUE(greater_than(MAX<signed>(), 3, 3));
        EXPECT_TRUE(greater_than(MAX<signed>(), 3, -3));
        EXPECT_TRUE(greater_than(MAX<signed>(), -3, 0));
        EXPECT_TRUE(greater_than(MAX<signed>(), -3, 3));
        EXPECT_TRUE(greater_than(MAX<signed>(), -3, -3));
    }
    ICY_SUBCASE("MIN, 0, 0") {
        EXPECT_FALSE(greater_than(MIN<signed>(), 3, 0));
        EXPECT_FALSE(greater_than(MIN<signed>(), 3, 3));
        EXPECT_FALSE(greater_than(MIN<signed>(), 3, -3));
        EXPECT_FALSE(greater_than(MIN<signed>(), -3, 0));
        EXPECT_FALSE(greater_than(MIN<signed>(), -3, 3));
        EXPECT_FALSE(greater_than(MIN<signed>(), -3, -3));
        EXPECT_FALSE(greater_than(MIN<signed>(), 0, 0));
        EXPECT_FALSE(greater_than(MIN<signed>(), 0, 3));
        EXPECT_FALSE(greater_than(MIN<signed>(), 0, -3));
    }
    ICY_SUBCASE("MAX, MAX, 0") {
        EXPECT_FALSE(greater_than(MAX<signed>(), MAX<signed>(), 0));
        EXPECT_FALSE(greater_than(MAX<signed>(), MAX<signed>(), 3));
        EXPECT_TRUE(greater_than(MAX<signed>(), MAX<signed>(), -3));
    }
    ICY_SUBCASE("MAX, MIN, 0") {
        EXPECT_TRUE(greater_than(MAX<signed>(), MIN<signed>(), 0));
        EXPECT_TRUE(greater_than(MAX<signed>(), MIN<signed>(), 3));
        EXPECT_TRUE(greater_than(MAX<signed>(), MIN<signed>(), -3));
    }
    ICY_SUBCASE("MIN, MAX, 0") {
        EXPECT_FALSE(greater_than(MIN<signed>(), MAX<signed>(), 0));
        EXPECT_FALSE(greater_than(MIN<signed>(), MAX<signed>(), 3));
        EXPECT_FALSE(greater_than(MIN<signed>(), MAX<signed>(), -3));
    }
    ICY_SUBCASE("MIN, MIN, 0") {
        EXPECT_FALSE(greater_than(MIN<signed>(), MIN<signed>(), 0));
        EXPECT_FALSE(greater_than(MIN<signed>(), MIN<signed>(), 3));
        EXPECT_TRUE(greater_than(MIN<signed>(), MIN<signed>(), -3));
    }
    ICY_SUBCASE("MAX, 0, MAX") {
        EXPECT_FALSE(greater_than(MAX<signed>(), 0, MAX<signed>()));
        EXPECT_FALSE(greater_than(MAX<signed>(), 3, MAX<signed>()));
        EXPECT_TRUE(greater_than(MAX<signed>(), -3, MAX<signed>()));
    }
    ICY_SUBCASE("MAX, 0, MIN") {
        EXPECT_TRUE(greater_than(MAX<signed>(), 0, MIN<signed>()));
        EXPECT_TRUE(greater_than(MAX<signed>(), 3, MIN<signed>()));
        EXPECT_TRUE(greater_than(MAX<signed>(), -3, MIN<signed>()));
    }
    ICY_SUBCASE("MIN, 0, MAX") {
        EXPECT_FALSE(greater_than(MIN<signed>(), 0, MAX<signed>()));
        EXPECT_FALSE(greater_than(MIN<signed>(), 3, MAX<signed>()));
        EXPECT_FALSE(greater_than(MIN<signed>(), -3, MAX<signed>()));
    }
    ICY_SUBCASE("MIN, 0, MIN") {
        EXPECT_FALSE(greater_than(MIN<signed>(), 0, MIN<signed>()));
        EXPECT_FALSE(greater_than(MIN<signed>(), 3, MIN<signed>()));
        EXPECT_TRUE(greater_than(MIN<signed>(), -3, MIN<signed>()));
    }
    ICY_SUBCASE("MAX, MAX, MAX") {
        EXPECT_FALSE(greater_than(MAX<signed>(), MAX<signed>(), MAX<signed>()));
    }
    ICY_SUBCASE("MAX, MAX, MIN") {
        EXPECT_TRUE(greater_than(MAX<signed>(), MAX<signed>(), MIN<signed>()));
    }
    ICY_SUBCASE("MAX, MIN, MAX") {
        EXPECT_TRUE(greater_than(MAX<signed>(), MIN<signed>(), MAX<signed>()));
    }
    ICY_SUBCASE("MAX, MIN, MIN") {
        EXPECT_TRUE(greater_than(MAX<signed>(), MIN<signed>(), MIN<signed>()));
    }
    ICY_SUBCASE("MIN, MAX, MAX") {
        EXPECT_FALSE(greater_than(MIN<signed>(), MAX<signed>(), MAX<signed>()));
    }
    ICY_SUBCASE("MIN, MAX, MIN") {
        EXPECT_FALSE(greater_than(MIN<signed>(), MAX<signed>(), MIN<signed>()));
    }
    ICY_SUBCASE("MIN, MIN, MAX") {
        EXPECT_FALSE(greater_than(MIN<signed>(), MIN<signed>(), MAX<signed>()));
    }
    ICY_SUBCASE("MIN, MIN, MIN") {
        EXPECT_TRUE(greater_than(MIN<signed>(), MIN<signed>(), MIN<signed>()));
    }
}
ICY_CASE("unsigned") {
    ICY_SUBCASE("MAX, 0, 0") {
        EXPECT_TRUE(greater_than(MAX<unsigned>(), 0u, 0u));
        EXPECT_TRUE(greater_than(MAX<unsigned>(), 0u, 3u));
        EXPECT_TRUE(greater_than(MAX<unsigned>(), 3u, 0u));
        EXPECT_TRUE(greater_than(MAX<unsigned>(), 3u, 3u));
    }
    ICY_SUBCASE("MAX, MAX, 0") {
        EXPECT_FALSE(greater_than(MAX<unsigned>(), MAX<unsigned>(), 0u));
        EXPECT_FALSE(greater_than(MAX<unsigned>(), MAX<unsigned>(), 3u));
    }
    ICY_SUBCASE("MAX, 0, MAX") {
        EXPECT_FALSE(greater_than(MAX<unsigned>(), 0u, MAX<unsigned>()));
        EXPECT_FALSE(greater_than(MAX<unsigned>(), 3u, MAX<unsigned>()));
    }
    ICY_SUBCASE("MAX, MAX, MAX") {
        EXPECT_FALSE(greater_than(MAX<unsigned>(), MAX<unsigned>(), MAX<unsigned>()));
        EXPECT_FALSE(greater_than(MAX<unsigned>(), MAX<unsigned>(), MAX<unsigned>()));
    }
    ICY_SUBCASE("0, 0, 0") {
        EXPECT_FALSE(greater_than(0u, 0u, 0u));
        EXPECT_FALSE(greater_than(0u, 0u, 3u));
        EXPECT_FALSE(greater_than(0u, 3u, 0u));
        EXPECT_FALSE(greater_than(0u, 3u, 3u));
        EXPECT_TRUE(greater_than(3u, 0u, 0u));
        EXPECT_FALSE(greater_than(3u, 0u, 3u));
        EXPECT_FALSE(greater_than(3u, 3u, 0u));
        EXPECT_FALSE(greater_than(3u, 3u, 3u));
    }
    ICY_SUBCASE("0, MAX, 0") {
        EXPECT_FALSE(greater_than(0u, MAX<unsigned>(), 0u));
        EXPECT_FALSE(greater_than(0u, MAX<unsigned>(), 3u));
        EXPECT_FALSE(greater_than(3u, MAX<unsigned>(), 0u));
        EXPECT_FALSE(greater_than(3u, MAX<unsigned>(), 3u));
    }
    ICY_SUBCASE("0, 0, MAX") {
        EXPECT_FALSE(greater_than(0u, 0u, MAX<unsigned>()));
        EXPECT_FALSE(greater_than(0u, 3u, MAX<unsigned>()));
        EXPECT_FALSE(greater_than(3u, 0u, MAX<unsigned>()));
        EXPECT_FALSE(greater_than(3u, 3u, MAX<unsigned>()));
    }
    ICY_SUBCASE("0, MAX, MAX") {
        EXPECT_FALSE(greater_than(0u, MAX<unsigned>(), MAX<unsigned>()));
        EXPECT_FALSE(greater_than(3u, MAX<unsigned>(), MAX<unsigned>()));
    }
}