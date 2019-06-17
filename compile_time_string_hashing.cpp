#include <iostream>

template<size_t N>
constexpr uint32_t HashImpl(const char (&str)[N], const size_t index);

template<size_t N>
constexpr uint32_t HashEntry(const char (&str)[N])
{
    return HashImpl<N>(str, N - 1);
}

template<size_t N>
constexpr uint32_t HashImpl(const char (&str)[N], const size_t index)
{
    if (index == 0u) return 2166136261u;

    return (HashImpl<N>(str, index - 1) ^ str[index - 1]) * 16777619u;
}

uint32_t Fnv1aHash(const char* str, int length)
{
    uint32_t hash = 2166136261u;

    // printf("[rut] (0) init: %u\n", hash);
    for (int i = 0; i < length; ++i)
    {
        const uint32_t value = static_cast<uint32_t>(*str++);
        hash ^= value;
        hash *= 16777619u;
        // printf("[rut] (%d) snip = %d, hashed: %u\n", i, value, hash);
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

    printf("hash (cmp): %x\n", hash1);
    printf("hash (rnt): %x\n", hash2);

    assert(hash1 == hash2);
}