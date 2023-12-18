#pragma once

#include "refl.h"

struct SubClass
{
    int d = 0;
};

struct MyClass
{
    REFL_DEFINE(MyClass);

    int a;
    int b;
    float c;

    REFL_OBJ_DECLARE(SubClass, sub);

    struct TypeInfo
    {
        static constexpr PropertyInfo<ReflBase> Properties[] = {
            REFL_PROP(a),
            REFL_PROP(b),
            REFL_PROP(c),
            REFL_OBJ(sub),
        };
    };
};
