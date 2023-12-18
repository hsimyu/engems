#pragma once

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

template <typename T>
void PrintMembers(const T &obj)
{
    const auto properties = std::span{T::TypeInfo::Properties};
    for (auto &&elem : properties)
    {
        switch (elem.pOffset.index())
        {
        case 0: // int
        {
            auto offset = std::get<0>(elem.pOffset);
            printf("- %s = %d\n", elem.name, obj.*offset);
            break;
        }
        case 1: // float
        {
            auto offset = std::get<1>(elem.pOffset);
            printf("- %s = %g\n", elem.name, obj.*offset);
            break;
        }
        }
    }
}
