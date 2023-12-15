/*
 * デバッグログ付きのカスタムアロケータの実装例
 */

#include <iostream>
#include <vector>
#include <memory>

using std::cout;
using std::endl;

template<class T>
class MyAllocator {
public:
    using value_type = T;

    MyAllocator() {}

    T* allocate(size_t n)
    {
        cout << "Allocate: " << typeid(T).name() << "(" << n << ")" << endl;
        return reinterpret_cast<T*>(std::malloc(sizeof(T) * n));
    }

    void deallocate(T* p, size_t n)
    {
        cout << "Deallocate: " << typeid(T).name() << "(" << p << ", " << n << ")" << endl;
        static_cast<void>(n); // unused 警告避け
        std::free(p);
    }
};

class MyClass {
public:
    MyClass() {
        cout << "My Class (" << value_ << ") is constructed!" << endl;
    }

    explicit MyClass(int v) : value_(v) {
        cout << "My Class (" << value_ << ") is constructed!" << endl;
    }

    MyClass(const MyClass& v) : value_(v.GetValue()) {
        cout << "My Class (" << value_ << ") is copy-constructed!" << endl;
    }

    MyClass(MyClass&& v) : value_(v.GetValue()) {
        cout << "My Class (" << value_ << ") is move-constructed!" << endl;
    }

    ~MyClass() {
        cout << "My Class (" << value_ << ") is destructed!" << endl;
    }

    void Print() {
        cout << "My name is MyClass with " << value_ << "." << endl;
    }

    int GetValue() const {
        return value_;
    }

private:
    int value_ = 0;
};

int main()
{
    auto v = new std::vector<MyClass, MyAllocator<MyClass>>;

    v->emplace_back(1);
    v->emplace_back(2);
    v->emplace_back(3);

    for(auto&& value : *v)
    {
        value.Print();
    }

    delete v;
    return 0;
}