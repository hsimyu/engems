#include <iostream>
#include <string_view>

template<size_t N>
constexpr uint32_t HashImpl(const char (&str)[N], const size_t index);

constexpr uint32_t HashImpl(const char* str, const size_t index);

// for C++14
template<size_t N>
constexpr uint32_t HashEntry(const char (&str)[N])
{
    return HashImpl(str, N - 1);
}

// for C++17
// string_view can catch both constexpr and runtime value
constexpr uint32_t HashEntry(const std::string_view& sv)
{
    return HashImpl(sv.data(), sv.length());
}

constexpr uint32_t HashImpl(const char* str, const size_t index)
{
    if (index == 0u) return 2166136261u;

    return (HashImpl(str, index - 1) ^ str[index - 1]) * 16777619u;
}

// for test
uint32_t Fnv1aHash(const char* str, int length)
{
    uint32_t hash = 2166136261u;

    for (int i = 0; i < length; ++i)
    {
        const uint32_t value = static_cast<uint32_t>(*str++);
        hash ^= value;
        hash *= 16777619u;
    }

    return hash;
}

uint32_t Fnv1aHash(const char* str)
{
    return Fnv1aHash(str, strlen(str));
}

int main()
{
    constexpr auto hash1 = HashEntry("hogeHash");
    const auto hash2 = Fnv1aHash("hogeHash");

    constexpr std::string_view sv("hogeHash");
    constexpr auto hash3 = HashEntry(sv);

    const std::string s("hogeHash");
    const auto hash4 = HashEntry(s.data());
    printf("hash (cmp): %x\n", hash1);
    printf("hash (rnt): %x\n", hash2);

    printf("hash (sv): %x\n", hash3);
    printf("hash (rnt): %x\n", hash4);

    assert(hash1 == hash2);
    assert(hash1 == hash3);
}