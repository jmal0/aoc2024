#include "read_txt.h"

#include <array>
#include <ranges>
#include <spanstream>
#include <string_view>
#include <sys/mman.h>

namespace aoc
{

//! Open a file for memory mapping and expose its contents as a string_view
struct Mmap_file
{
    explicit Mmap_file(const char* filename)
        : length_{std::filesystem::file_size(filename)}
    {
        auto fd = std::fopen(filename, "rb");
        addr_ = mmap(nullptr, length_, PROT_READ, MAP_FILE | MAP_PRIVATE, fileno(fd), 0);
        std::fclose(fd);
    }

    Mmap_file(const Mmap_file&) = delete;
    Mmap_file(Mmap_file&&) = delete;
    Mmap_file& operator=(const Mmap_file&) = delete;
    Mmap_file& operator=(Mmap_file&&) = delete;

    ~Mmap_file()
    {
        if (addr_)
        {
            ::munmap(addr_, length_);
        }
    }

    [[nodiscard]] explicit operator bool () const noexcept
    {
        return addr_ != nullptr;
    }

    [[nodiscard]] std::string_view contents() const noexcept
    {
        return {static_cast<const char*>(addr_), length_};
    }
private:
    std::size_t length_{};
    void* addr_{};
};

template <typename T>
struct Line_converter;

template <>
struct Line_converter<std::string>
{
    static auto operator()(const auto& line) -> std::string
    {
        return std::string{line.begin(), line.end()};
    }
};

template <typename T1, typename T2>
struct Line_converter<std::pair<T1, T2>>
{
    static auto operator()(const auto& line) -> std::pair<T1, T2>
    {
        auto ret = std::pair<T1, T2>{};
        auto linestream = std::ispanstream{line};
        if (linestream >> ret.first && linestream >> ret.second)
        {
            return ret;
        }
        throw std::runtime_error{"Bad input line: " + std::string(line.begin(), line.end())};
    }
};

template <typename T, std::size_t N>
struct Line_converter<std::array<T, N>>
{
    static auto operator()(const auto& line) -> std::array<T, N>
    {
        auto ret = std::array<T, N>{};
        auto linestream = std::ispanstream{line};
        for (auto& val : ret)
        {
            if (!(linestream >> val))
            {
                throw std::runtime_error{"Bad input line: " + std::string(line.begin(), line.end())};
            }
        }
        return ret;
    }
};

template <typename T>
struct Line_converter<std::vector<T>>
{
    static auto operator()(const auto& line) -> std::vector<T>
    {
        auto ret = std::vector<T>{};
        ret.reserve(64);
        auto linestream = std::ispanstream{line};
        T val;
        while (linestream >> val)
        {
            ret.emplace_back(val);
        }
        return ret;
    }
};

template <typename T>
auto read_txt(const std::filesystem::path& filename) -> std::vector<T>
{
    auto file = Mmap_file{filename.c_str()};
    auto contents = file.contents();
    return contents
        | std::views::split('\n')
        | std::views::filter([](const auto& line) { return !line.empty(); })
        | std::views::transform(Line_converter<T>{})
        | std::ranges::to<std::vector<T>>();
}

template auto read_txt<std::string>(const std::filesystem::path& filename) -> std::vector<std::string>;
template auto read_txt<std::pair<int, int>>(const std::filesystem::path& filename) -> std::vector<std::pair<int, int>>;
template auto read_txt<std::pair<long, long>>(const std::filesystem::path& filename) -> std::vector<std::pair<long, long>>;
template auto read_txt<std::pair<double, double>>(const std::filesystem::path& filename) -> std::vector<std::pair<double, double>>;
template auto read_txt<std::array<double, 5>>(const std::filesystem::path& filename) -> std::vector<std::array<double, 5>>;
template auto read_txt<std::vector<int>>(const std::filesystem::path& filename) -> std::vector<std::vector<int>>;

} // namespace aoc
