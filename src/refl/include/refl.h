#pragma once

#include <cstdio>
#include <cstddef>
#include <span>
#include <variant>
#include <string_view>

enum class PropertyType
{
    Invalid,
    Int,
    Float,
    Obj,
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

struct ObjDesc
{
};

template <typename T>
using MemberPtrVariant = std::variant<int T::*, float T::*, ObjDesc T::*>;

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
PropertyInfo<T> FindMemberInfo(std::string_view name)
{
    const auto properties = std::span{T::TypeInfo::Properties};
    auto itr = std::find_if(properties.begin(), properties.end(),
                            [name](const PropertyInfo<T> &elem)
                            {
                                return name == elem.name;
                            });

    if (itr == properties.end())
    {
        return {.t = PropertyType::Invalid};
    }

    return *itr;
}

// template <typename T>
// void PrintMemberValue(const T &obj, const PropertyInfo<T> &info)
// {
//     switch (info.pOffset.index())
//     {
//     case 0: // int
//     {
//         auto offset = std::get<0>(info.pOffset);
//         printf("%d\n", obj.*offset);
//         break;
//     }
//     case 1: // float
//     {
//         auto offset = std::get<1>(info.pOffset);
//         printf("%g\n", obj.*offset);
//         break;
//     }
//     case 2: // Obj
//     {
//         printf("base address = %p\n", &obj);
//         auto offset = std::get<2>(info.pOffset);
//         auto address = &(obj.*offset);
//         printf("subObj address = %p\n", address);
//         break;
//     }
//     }
// }

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
        case 2: // obj
        {
            auto offset = std::get<2>(elem.pOffset);
            printf("- %s = %p\n", elem.name, &(obj.*offset));
            break;
        }
        }
    }
}
