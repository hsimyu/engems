#pragma once

#include "refl.h"

struct SubClass
{
    int d = 0;
};

struct MyClass
{
    int a;
    int b;
    float c;

    union
    {
        ObjDesc subAddress;
        SubClass sub;
    };

    struct TypeInfo
    {
        using T = MyClass;
        static constexpr PropertyInfo<MyClass> Properties[] = {
            REFL_PROP(a),
            REFL_PROP(b),
            REFL_PROP(c),
            PropertyInfo<MyClass>{
                PropertyType::Obj,
                &MyClass::subAddress,
                "sub"},
        };
    };
};
