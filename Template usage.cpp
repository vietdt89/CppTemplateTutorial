#include <iostream>

using namespace std;

int a;
struct B { int v; };
class C{
public:
    int s_mem;
};

template <typename T> struct X {
    B b;
    T t;
    X* anthor;
    typedef int Y;
    Y y;
    void foo() {
       b.v += y;
       b.v *= T::s_mem;   //no error
    }
};

struct X<int> x; // no error
struct X<C> y;

template <typename T> struct DoWork;	 // struct with 1 typename

template <> struct DoWork<int> {};
template <> struct DoWork<float> {};
//template <> struct DoWork<int, int> {};  // (3) error for 2 typename

void f(){
    DoWork<int>      i;
    DoWork<float>    f;
    //DoWork<int, int> ii; // (3) error for 2 typename
}

template <typename T, typename U> struct S            ;    // 0
template <typename T>             struct S<T,  T  > {};    // 1
template <typename T>             struct S<T*, T  > {};    // 2
template <typename T>             struct S<T,  T* > {};    // 3
template <typename U>             struct S<U,  int> {};    // 4
template <typename U>             struct S<U*, int> {};    // 5
template <typename U, typename T> struct S<U*, T* > {};    // 6
template <typename U, typename T> struct S<U,  T* > {};    // 7
//template <typename T>             struct S<unique_ptr<T>, shared_ptr<T>>; // 8

S<float*,  int>      v0;
S<double*, int>      v1;
S<double,  double>   v2;
S<float*,  double*>  v3;
//S<float*,  float*>   v4;  // error no struct
S<double,  float*>   v5;
S<int,     double*>  v6;
//S<int*,    int>      v7; // error no struct
S<double*, double>   v8;

int main()
{
    //x.foo();
    //y.foo();
    return 0;
}
