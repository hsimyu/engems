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

template <typename T>
struct PropertyInfo
{
    using MemberPtr = typename int T::*;
    PropertyType t;
    MemberPtr pOffset;
    const char *name;
};

struct MyClass
{
    int a;
    int b;
    float c;
};

#define REFL_PROP(name)                         \
    PropertyInfo<T>                             \
    {                                           \
        GetPropertyType<decltype(T::##name)>(), \
            &T::##name,                         \
            #name                               \
    }

template <typename T>
struct TypeInfo
{
    static constexpr PropertyInfo<T> Properties[] = {};
};

template <>
struct TypeInfo<MyClass>
{
    using T = MyClass;
    static constexpr PropertyInfo<T> Properties[] = {
        REFL_PROP(a),
        REFL_PROP(b),
        // REFL_PROP(c),
    };
};

int main()
{
    [[maybe_unused]] auto p1 = &MyClass::a;
    constexpr auto view = std::span{TypeInfo<MyClass>::Properties};

    for (auto &&elem : view)
    {
        printf("Name = %s, offset = %p\n", elem.name, elem.pOffset);
    }

    MyClass c{};
    c.a = 1;
    c.b = 12;
    c.c = 32;
    MyClass *p = &c;

    printf("BaseAddress = %p\n", p);
    uintptr_t base = reinterpret_cast<uintptr_t>(p);

    for (auto &&elem : view)
    {
        auto memberAddress = base;
        const auto offset = elem.pOffset;
        printf("- %s = %p + %zu = %d\n", elem.name, p, elem.pOffset, p->*offset);
    }

    return 0;
}