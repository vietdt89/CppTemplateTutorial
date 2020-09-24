#include <iostream>

using namespace std;

template <int i> class A
{
public:
    void foo(int)
    {
    }
};
template <uint8_t a, typename b, void* c> class B {};
template <bool, void (*a)()> class C {};
template <void (A<3>::*a)(int)> class D {};

template <int i> int Add(int a)
{
    return a + i;
}

void foo()
{
    A<5> a;
    B<7, A<5>, nullptr>	b;
    C<false, &foo> c;
    D<&A<3>::foo> d;
    int x = Add<3>(5);     // x == 8, integer template param can only be obtained manually
}

//template <float a> class E {}; // ERROR:only integer

int main()
{

    return 0;
}
