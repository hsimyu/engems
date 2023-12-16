#include <cstdio>
#include <cstddef>
#include <span>

enum class PropertyType
{
    Int,
    Float
};

template <typename T>
constexpr PropertyType GetPropertyType();

template <>
constexpr PropertyType GetPropertyType<int>()
{
    return PropertyType::Int;
}

template <>
constexpr PropertyType GetPropertyType<float>()
{
    return PropertyType::Float;
}

struct TypeDesc
{
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

#define REFL_PROP(name)                         \
    PropertyInfo                                \
    {                                           \
        GetPropertyType<decltype(T::##name)>(), \
            offsetof(T, name),                  \
            #name                               \
    }

template <typename T>
struct TypeInfo
{
    static constexpr PropertyInfo Properties[] = {};
};

template <>
struct TypeInfo<MyClass>
{
    using T = MyClass;
    static constexpr PropertyInfo Properties[] = {
        REFL_PROP(a),
        REFL_PROP(b),
        REFL_PROP(c),
    };
};

int main()
{
    constexpr auto view = std::span{TypeInfo<MyClass>::Properties};

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