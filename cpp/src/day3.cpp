#include <algorithm>
#include <charconv>
#include <optional>
#include <ranges>
#include <string_view>
#include <vector>

namespace aoc
{

constexpr auto get_mul_arg(std::string_view& string_starting_at_number, char terminator) -> std::optional<int>
{
    const auto orig_string_view = string_starting_at_number;
    constexpr auto is_digit = [](char c) { return c >= '0' && c <= '9'; };

    const auto first_nonnumeric = std::ranges::find_if_not(string_starting_at_number, is_digit);
    if (first_nonnumeric == string_starting_at_number.end() || *first_nonnumeric != terminator)
    {
        // Incomplete
        string_starting_at_number = std::string_view{first_nonnumeric, string_starting_at_number.end()};
        return std::nullopt;
    }
    int num{};
    std::from_chars(orig_string_view.begin(), first_nonnumeric, num);
    string_starting_at_number = std::string_view{first_nonnumeric + 1, string_starting_at_number.end()};
    return num;
}

constexpr auto advance_to_end_of_mul(std::string_view& string_after_mul) -> int
{
    const auto left = get_mul_arg(string_after_mul, ',');
    if (left == std::nullopt)
    {
        // Incomplete, return 0 so total is unchanged
        return 0;
    }
    const auto right = get_mul_arg(string_after_mul, ')');
    if (right == std::nullopt)
    {
        // Incomplete, return 0 so total is unchanged
        return 0;
    }

    return *left * *right;
}

constexpr auto do_muls(std::string_view data) -> int
{
    using namespace std::string_view_literals;
    constexpr auto pattern = "mul("sv;
    int sum = 0;
    while (!data.empty())
    {
        auto next_mul_ind = data.find(pattern);
        if (next_mul_ind == std::string_view::npos)
        {
            break;
        }
        data = data.substr(next_mul_ind + std::ssize(pattern));
        sum += advance_to_end_of_mul(data);
    }
    return sum;
}

struct Toggle_instruction
{
    long location{};
    bool enabled{};
};

constexpr auto find_toggle_instructions(std::string_view data) -> std::vector<Toggle_instruction>
{
    const char* orig_str = data.data();
    auto toggle_locs = std::vector<Toggle_instruction>{};
    toggle_locs.reserve(data.size());
    toggle_locs.emplace_back(Toggle_instruction{.location = -1, .enabled = true}); // enabled at start
    while (!data.empty())
    {
        auto next_do_ind = data.find("do");
        if (next_do_ind == std::string_view::npos)
        {
            break;
        }
        data = data.substr(next_do_ind + 2);
        if (data.starts_with("()"))
        {
            toggle_locs.emplace_back(Toggle_instruction{.location = data.data() - orig_str, .enabled = true});
        }
        else if (data.starts_with("n't()"))
        {
            toggle_locs.emplace_back(Toggle_instruction{.location = data.data() - orig_str, .enabled = false});
        }

    }
    return toggle_locs;
}

constexpr auto do_muls_with_toggle(std::string_view data) -> int
{
    const char* orig_str = data.data();
    const auto toggle_locs = find_toggle_instructions(data);

    using namespace std::string_view_literals;
    constexpr auto pattern = "mul("sv;
    int sum = 0;
    while (!data.empty())
    {
        auto next_mul_ind = data.find(pattern);
        if (next_mul_ind == std::string_view::npos)
        {
            break;
        }
        data = data.substr(next_mul_ind + std::ssize(pattern));
        auto prod = advance_to_end_of_mul(data);
        auto enable_it = std::ranges::upper_bound(
            toggle_locs, data.data() - orig_str, {}, &Toggle_instruction::location) - 1;
        sum += enable_it->enabled ? prod : 0;
    }
    return sum;
}

static_assert(do_muls("xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))") == 161);
static_assert(do_muls_with_toggle("xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))") == 48);
static_assert(do_muls_with_toggle("mul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))") == 48);
static_assert(do_muls_with_toggle("do()mul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))") == 48);
static_assert(do_muls_with_toggle("don't()mul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))") == 40);

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

    auto data = aoc::read_txt_into_string(argv[1]);
    std::cout << aoc::do_muls(data) << '\n';
    std::cout << aoc::do_muls_with_toggle(data) << '\n';
    return 0;
}