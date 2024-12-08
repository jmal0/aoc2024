#ifndef AOC2024_READ_TXT_H
#define AOC2024_READ_TXT_H

#include <filesystem>
#include <iosfwd>
#include <string>
#include <vector>

namespace aoc
{

template <typename T>
auto read_txt(const std::filesystem::path& filename) -> std::vector<T>;

} // namespace aoc

#endif // AOC2024_READ_TXT_H
