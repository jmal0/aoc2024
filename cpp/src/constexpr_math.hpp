#ifndef AOC2024_CONSTEXPR_MATH_HPP
#define AOC2024_CONSTEXPR_MATH_HPP

namespace aoc
{

template <typename T>
constexpr auto abs(T x) -> T
{
    return x < 0 ? -x : x;
}

} // namespace aoc

#endif // AOC2024_CONSTEXPR_MATH_HPP
