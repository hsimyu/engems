#include <cstdio>
#include <cstddef>
#include <span>
#include <variant>

#include <refl.h>
#include <MyClass.h>

int main()
{
    MyClass c{};
    c.a = 1;
    c.b = 12;
    c.c = 32;

    PrintMembers(c);

    return 0;
}