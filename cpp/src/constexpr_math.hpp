#ifndef AOC2024_CONSTEXPR_MATH_HPP
#define AOC2024_CONSTEXPR_MATH_HPP

namespace aoc
{

template <typename T>
constexpr auto abs(T x) -> T
{
    return x < 0 ? -x : x;
}

template <typename T>
constexpr auto sign(T x) -> int
{
    return x > 0 ? 1 : (x < 0) ? -1 : 0;
}

} // namespace aoc

#endif // AOC2024_CONSTEXPR_MATH_HPP
