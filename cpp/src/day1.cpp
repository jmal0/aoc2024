#include <array>
#include <algorithm>
#include <ranges>
#include <span>
#include <utility>

#include "constexpr_math.hpp"

namespace aoc
{

constexpr auto get_pairs_distance(std::span<std::pair<long, long>> pairs) -> long
{
    using std::views::transform;
    // Sort left elements of the pairs
    std::ranges::sort(pairs | transform([](auto& pair) -> long& { return pair.first; }));
    // Sort right elements of the pairs
    std::ranges::sort(pairs | transform([](auto& pair) -> long& { return pair.second; }));
    // Sum the differences of the now two sides, which are both in increasing order
    auto pair_diff = [](const auto& pair) { return abs(pair.first - pair.second); };
    return std::ranges::fold_left(pairs | transform(pair_diff), 0, std::plus<>{});
}

constexpr auto get_similarity_score(std::span<const std::pair<long, long>> pairs) -> long
{
    long score = 0;
    for (const auto& val : pairs | std::views::elements<0>)
    {
        score += val * std::ranges::count(pairs | std::views::elements<1>, val);
    }
    return score;
}

static constexpr auto test_input = std::array{
    std::pair{3L, 4L},
    std::pair{4L, 3L},
    std::pair{2L, 5L},
    std::pair{1L, 3L},
    std::pair{3L, 9L},
    std::pair{3L, 3L},
};

consteval auto test_part1() -> long
{
    auto copy = test_input;
    return get_pairs_distance(copy);
}

static_assert(test_part1() == 11);

consteval auto test_part2() -> long
{
    return get_similarity_score(test_input);
}

static_assert(test_part2() == 31);

} // namespace aoc

#include <iostream>
#include "read_txt.h"

int main(int argc, const char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./day1 [input]\n";
        return 1;
    }
    auto data = aoc::read_txt<std::pair<long, long>>(argv[1]);
    std::cout << aoc::get_pairs_distance(data) << '\n';
    std::cout << aoc::get_similarity_score(data) << '\n';
    return 0;
}