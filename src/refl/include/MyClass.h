#pragma once

#include "refl.h"

struct SubClass
{
    int d = 0;

    REFL_DEFINE(SubClass);
};

struct SubClass::TypeInfo
{
    static constexpr PropertyInfo Properties[] = {
        REFL_PROP(d),
    };
};

struct MyClass
{
    REFL_DEFINE(MyClass);

    int a;
    int b;
    float c;

    SubClass sub;
};

struct MyClass::TypeInfo
{
    static constexpr PropertyInfo Properties[] = {
        REFL_PROP(a),
        REFL_PROP(b),
        REFL_PROP(c),
        REFL_OBJ(sub),
    };
};
