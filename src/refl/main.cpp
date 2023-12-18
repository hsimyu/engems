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
    c.sub.d = 48;

    PrintMembers(c);

    {
        auto info = FindMemberInfo<MyClass>("b");
        if (info.t != PropertyType::Invalid)
        {
            printf("- name = %s, type = %d\n", info.name, info.t);
            auto offset = std::get<0>(info.pOffset);
            printf("- value = %d\n", c.*offset); // 12
        }
    }

    {
        auto info = FindMemberInfo<MyClass>("sub");
        if (info.t != PropertyType::Invalid)
        {
            printf("- name = %s, type = %d\n", info.name, info.t);

            auto offset = std::get<2>(info.pOffset);
            auto address = &(c.*offset);
            printf("subObj address = %p\n", address);

            auto *pSubObj = reinterpret_cast<SubClass *>(address);
            printf("subObj value = %d\n", pSubObj->d);
        }
    }

    return 0;
}