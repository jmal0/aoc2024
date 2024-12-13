#include <array>
#include <algorithm>
#include <ranges>
#include <span>
#include <vector>

#include "constexpr_math.hpp"

namespace aoc
{

using Report = std::span<const int>;

constexpr auto is_safe(const Report& report) -> bool
{
    if (report.size() <= 1)
    {
        return true; // 0 or 1 elements can't violate rules for adjacent differences
    }
    auto diffs = report | std::views::adjacent_transform<2>([](auto left, auto right) { return left - right; } );
    auto [min_it, max_it] = std::ranges::minmax_element(diffs);
    return (*min_it >= 1 && *max_it <= 3) || (*min_it >= -3 && *max_it <= -1);
}

template <std::ranges::range Range_of_report_like>
constexpr auto count_safe_reports(const Range_of_report_like& reports) -> long
{
    return std::ranges::count_if(reports, is_safe);
}

constexpr auto is_safe_with_dampening(const Report& report) -> bool
{
    // Fuck it, brute force
    if (is_safe(report))
    {
        return true;
    }
    // Test each removal individually
    for (int i = 0; i < std::ssize(report); ++i)
    {
        auto report_copy = std::vector<int>{report.begin(), report.end()};
        if (i < std::ssize(report) - 1)
        {
            auto first_it = report_copy.begin() + i;
            std::rotate(first_it, first_it + 1, report_copy.end());
        }
        if (is_safe(std::span{report_copy}.first(report_copy.size() - 1)))
        {
            return true;
        }
    }
    return false; // No removal worked. Not safe
}

template <std::ranges::range Range_of_report_like>
constexpr auto count_dampened_safe_reports(const Range_of_report_like& reports) -> long
{
    return std::ranges::count_if(reports, is_safe_with_dampening);
}

static constexpr auto test_input = std::array{
    std::array{7, 6, 4, 2, 1},
    std::array{1, 2, 7, 8, 9},
    std::array{9, 7, 6, 2, 1},
    std::array{1, 3, 2, 4, 5},
    std::array{8, 6, 4, 4, 1},
    std::array{1, 3, 6, 7, 9},
};

static_assert(is_safe(test_input.at(0)));
static_assert(!is_safe(test_input.at(1)));
static_assert(!is_safe(test_input.at(2)));
static_assert(!is_safe(test_input.at(3)));
static_assert(!is_safe(test_input.at(4)));
static_assert(is_safe(test_input.at(5)));

static_assert(is_safe_with_dampening(test_input.at(0)));
static_assert(!is_safe_with_dampening(test_input.at(1)));
static_assert(!is_safe_with_dampening(test_input.at(2)));
static_assert(is_safe_with_dampening(test_input.at(3)));
static_assert(is_safe_with_dampening(test_input.at(4)));
static_assert(is_safe_with_dampening(test_input.at(5)));
static_assert(is_safe_with_dampening(std::array{4, 4})); // Not sure what is considered correct here
static_assert(!is_safe_with_dampening(std::array{4, 4, 4}));
static_assert(is_safe_with_dampening(std::array{4, 4, 7}));
static_assert(!is_safe_with_dampening(std::array{4, 4, 4, 7}));
// Thanks https://www.reddit.com/r/adventofcode/comments/1h4shdu/2024_day_2_part2_edge_case_finder/
static_assert(is_safe_with_dampening(std::array{48, 46, 47, 49, 51, 54, 56}));
static_assert(is_safe_with_dampening(std::array{1, 1, 2, 3, 4, 5}));
static_assert(is_safe_with_dampening(std::array{1, 2, 3, 4, 5, 5}));
static_assert(is_safe_with_dampening(std::array{5, 1, 2, 3, 4, 5}));
static_assert(is_safe_with_dampening(std::array{1, 4, 3, 2, 1}));
static_assert(is_safe_with_dampening(std::array{1, 6, 7, 8, 9}));
static_assert(is_safe_with_dampening(std::array{1, 2, 3, 4, 3}));
static_assert(is_safe_with_dampening(std::array{9, 8, 7, 6, 7}));
static_assert(is_safe_with_dampening(std::array{7, 10, 8, 10, 11}));
static_assert(is_safe_with_dampening(std::array{29, 28, 27, 25, 26, 25, 22, 20}));

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

    auto data = aoc::read_txt<std::vector<int>>(argv[1]);
    std::cout << aoc::count_safe_reports(data) << '\n';
    std::cout << aoc::count_dampened_safe_reports(data) << '\n';
    return 0;
}