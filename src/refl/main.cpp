#include <cstdio>
#include <cstddef>
#include <span>

enum class PropertyType
{
    Int,
    Float
};

struct PropertyInfo
{
    PropertyType t;
    size_t offset;
    const char *name;
};

struct MyClass
{
    int a;
    int b;
    float c;
};

static constexpr PropertyInfo Properties[3] = {
    PropertyInfo{
        PropertyType::Int,
        offsetof(MyClass, a),
        "a"},
    PropertyInfo{
        PropertyType::Int,
        offsetof(MyClass, b),
        "b"},
    PropertyInfo{
        PropertyType::Float,
        offsetof(MyClass, c),
        "c"},
};
int main()
{
    constexpr auto view = std::span{Properties};

    for (auto &&elem : view)
    {
        printf("Name = %s, offset = %zu\n", elem.name, elem.offset);
    }

    MyClass c{};
    MyClass *p = &c;

    printf("BaseAddress = %p\n", p);
    uintptr_t base = reinterpret_cast<uintptr_t>(p);

    for (auto &&elem : view)
    {
        auto memberAddress = base + elem.offset;
        printf("- %s = %p + %zu = %p\n", elem.name, p, elem.offset, reinterpret_cast<void *>(memberAddress));
    }

    return 0;
}