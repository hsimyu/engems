#include <cstdio>
#include <cstddef>
#include <span>
#include <variant>

#include <MyClass.h>

int main()
{
    constexpr auto view = std::span{MyClass::TypeInfo::Properties};

    MyClass c{};
    c.a = 1;
    c.b = 12;
    c.c = 32;
    MyClass *p = &c;

    printf("BaseAddress = %p\n", p);
    uintptr_t base = reinterpret_cast<uintptr_t>(p);

    for (auto &&elem : view)
    {
        auto memberAddress = base;

        switch (elem.pOffset.index())
        {
        case 0: // int
        {
            auto offset = std::get<0>(elem.pOffset);
            printf("- %s = %d\n", elem.name, p->*offset);
            break;
        }
        case 1: // float
        {
            auto offset = std::get<1>(elem.pOffset);
            printf("- %s = %g\n", elem.name, p->*offset);
            break;
        }
        }
    }

    return 0;
}