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

struct PropertyInfo
{
    PropertyType t;
    size_t offset;
    const char *name;
    std::span<const PropertyInfo> members{};
};

#define REFL_DEFINE(typename)  \
    using ReflBase = typename; \
    struct TypeInfo

#define REFL_OBJ(name)                                                   \
    PropertyInfo                                                         \
    {                                                                    \
        PropertyType::Obj,                                               \
            offsetof(ReflBase, name),                                    \
            #name,                                                       \
            std::span{decltype(ReflBase::##name)::TypeInfo::Properties}, \
    }

#define REFL_PROP(name)                                \
    PropertyInfo                                       \
    {                                                  \
        GetPropertyType<decltype(ReflBase::##name)>(), \
            offsetof(ReflBase, name),                  \
            #name,                                     \
            {},                                        \
    }

template <typename T>
PropertyInfo FindMemberInfo(std::string_view name)
{
    const auto properties = std::span{T::TypeInfo::Properties};
    auto itr = std::find_if(properties.begin(), properties.end(),
                            [name](const PropertyInfo &elem)
                            {
                                return name == elem.name;
                            });

    if (itr == properties.end())
    {
        return {.t = PropertyType::Invalid};
    }

    return *itr;
}

template <typename T>
void PrintMembers(const T &obj)
{
    PrintMembersImpl(reinterpret_cast<uintptr_t>(&obj), std::span{T::TypeInfo::Properties});
}

inline void PrintMembersImpl(uintptr_t address, std::span<const PropertyInfo> properties)
{
    for (auto &&elem : properties)
    {
        uintptr_t memberAddress = address + elem.offset;
        switch (elem.t)
        {
        case PropertyType::Int:
        {
            auto *pMember = reinterpret_cast<int *>(memberAddress);
            printf("- [int] %s = %d\n", elem.name, *pMember);
            break;
        }
        case PropertyType::Float:
        {
            auto *pMember = reinterpret_cast<float *>(memberAddress);
            printf("- [float] %s = %g\n", elem.name, *pMember);
            break;
        }
        case PropertyType::Obj:
        {
            auto *pMember = reinterpret_cast<void *>(memberAddress);
            printf("- [obj] %s = %p\n", elem.name, pMember);
            PrintMembersImpl(reinterpret_cast<uintptr_t>(pMember), elem.members);
            break;
        }
        }
    }
}
