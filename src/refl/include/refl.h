#pragma once

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