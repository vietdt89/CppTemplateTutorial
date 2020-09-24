#include <iostream>

using namespace std;

template <typename T>
class vector1
{
public:
    //implement inlinde
    void clear()
    {
        // Function body
    }

private:
    T* elements;
};

template <typename T>
class vector2
{
public:
    void clear();
private:
    T* elements;
};
//implement not inline
template <typename T>
void vector2<T>::clear()
{
    // Function body
}

//template function declaration
template <typename T>
void foo(T const& v);

template <typename T>
T foo();

template <typename T, typename U>
U foo(T const&);

template <typename T>
void foo()
{
    T var;
    // ...
}

//template function usage
template <typename T> T Add(T a, T b)
{
    return a + b;
}

template <typename T> char Add2(T a, T b)
{
    return a + b;
}

template <int i> class A {};

void foo()
{
    int x = 3;
    A<5> a; // ok
    //A<x> b; // error, i must be determined at compile time
}

template <typename T, int V> class TemplateWithValue{
public:
    T data[V];
};

int main()
{
    int a = 3;
    char b = '4';
    cout << Add<int> (a,b);    //--> 55
    cout << Add2<char> (a,b);   //--> 7

    TemplateWithValue<char,10> arr;
    cout << sizeof(arr.data)/sizeof(arr.data)[0];  // --> 10
    return 0;
}
