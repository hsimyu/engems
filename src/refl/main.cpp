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

    auto info = FindMemberInfo<MyClass>("b");
    if (info.t != PropertyType::Invalid)
    {
        printf("- name = %s, type = %d\n", info.name, info.t);
        PrintMemberValue(c, info); // -> 12
    }

    return 0;
}