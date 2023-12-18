#include <cstdio>
#include <cstddef>
#include <span>
#include <variant>

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
using MemberPtrVariant = std::variant<int T::*, float T::*>;

template <typename T>
struct PropertyInfo
{
    PropertyType t;
    MemberPtrVariant<T> pOffset;
    const char *name;
};

#define REFL_PROP(name)                         \
    PropertyInfo<T>                             \
    {                                           \
        GetPropertyType<decltype(T::##name)>(), \
            &T::##name,                         \
            #name                               \
    }

struct MyClass
{
    int a;
    int b;
    float c;

    struct TypeInfo;
};

struct MyClass::TypeInfo
{
    using T = MyClass;
    static constexpr PropertyInfo<MyClass> Properties[] = {
        REFL_PROP(a),
        REFL_PROP(b),
        REFL_PROP(c),
    };
};

int main()
{
    constexpr auto view = std::span{MyClass::TypeInfo::Properties};

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

        switch (elem.pOffset.index())
        {
        case 0: // int
        {
            auto offset = std::get<0>(elem.pOffset);
            printf("- %s = %d\n", elem.name, p->*offset);
            break;
        }
        case 1: // float
        {
            auto offset = std::get<1>(elem.pOffset);
            printf("- %s = %g\n", elem.name, p->*offset);
            break;
        }
        }
    }

    return 0;
}