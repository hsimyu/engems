#pragma once

#include "refl.h"

struct MyClass
{
    int a;
    int b;
    float c;

    struct TypeInfo
    {
        using T = MyClass;
        static constexpr PropertyInfo<MyClass> Properties[] = {
            REFL_PROP(a),
            REFL_PROP(b),
            REFL_PROP(c),
        };
    };
};
