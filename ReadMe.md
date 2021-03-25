# C++ Template guide
## 1. Template basic syntax
C++ template metaprogramming uses template instantiation to drive compile-time evaluation. Wehn we use the name of a template where a function, type, variable is expected, the compiler will instantiate the expected entity from that template. Templete improve performance and sourcecode flexibility

Template Class statement：
```C++
template <typename T> class ClassA;
```

Template Class definition：
```C++
template <typename T> class ClassA
{
	T member;
};
```
Example

``` C++
template <typename T>
class A{
public:
    T a;
};

void main(){
    A<char> a;
    std::cout << typeid (a.a).name() << std::endl;
}
```
Printout
``` C++
c
```
Template class definition full version.
```C++
template <typename T>
class vector
{
public:
	void push_back(T const&);
	void clear();				
	
private:
	T* elements;
};
```
```C++
vector<int> intArray;
vector<float> floatArray;
```
```C++
intArray.push_back(5);
floatArray.push_back(3.0f);
```
Temlate can not be used directly to define variables

```C++
vector unknownVector; // wrong
```
```C++
vector<int>
ClassA<double>

template <typename T0, typename T1> class ClassB
{
    // Class body ...
};

ClassB<int, float>
```
## 2. Template class function definition
Member functions in template class are usually implemented inline

``` C++
template <typename T>
class vector
{
public:
    void clear()
    {
        // Function body
    }
	
private:
    T* elements;
};
```
Not inline version
```C++
template <typename T>
class vector
{
public:
    void clear();  
private:
    T* elements;
};

template <typename T>
void vector<T>::clear() 
{
	// Function body
}
```
## 3. Basic Template Function
```C++
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
```
#### Template function usage

``` C++
template <typename T> T Add(T a, T b)
{
    return a + b;
}
```
This can still work
``` C++
template<typename T> T Add(T a, T b){
    return a+b;
}

int main()
{
    int a = 3;
    char b = '4';
    std::cout << Add<int> (a,b);
    return 0;
}
```
### Template with int type
``` C++
template <typename T> class TemplateWithType;
template <int      V> class TemplateWithValue;
```
``` C++
template <typename T, int Size> struct Array
{
    T data[Size];
};

Array<int, 16> arr;
```

This is equivalent to 

``` C++
class IntArrayWithSize16
{
    int data[16]; // int 替换了 T, 16 替换了 Size
};

IntArrayWithSize16 arr;
```
``` C++
template <int i> class A {};

void foo()
{
    int x = 3;
    A<5> a; // ok
    A<x> b; // error, i must be determined at compile time
}
```
``` C++
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

template <float a> class E {}; // ERROR:only integer
```

## 4  Basic meta programming
Let's look at an example for basic Stack implementation using template
``` C++
template <typename T>
class Stack
{
public:
    void push(T v);
    T pop();
    int Find(T x)
    {
        for(int i = 0; i < size; ++i)
        {
            if(data[i] == x) { return i; }
        }
    }
};

typedef Stack<int>   StackInt;
typedef Stack<float> StackFloat;
```
Look at the example. We want to addFloatOrMulInt to behave differently on int/float input
``` C
struct Variant
{
    union
    {
        int x;
        float y;
    } data;
    uint32 typeId;
};

Variant addFloatOrMulInt(Variant const* a, Variant const* b)
{
    Variant ret;
    assert(a->typeId == b->typeId);
    if (a->typeId == TYPE_INT)
    {
        ret.x = a->x * b->x;
    }
    else
    {
        ret.y = a->y + b->y;
    }
    return ret;
}

```

Sovle the problem by template：

``` C++
template <typename T> 
class AddFloatOrMulInt
{
    T Do(T a, T b)
    {
        return a + b;
    }
};

template <typename T> 
class AddFloatOrMulInt
{
    T Do(T a, T b)
    {
        return a * b;
    }
};

void foo()
{
    float a, b, c;

    c = AddFloatOrMulInt<float>::Do(a, b);
}
```
``` C++
template <typename T> 
class AddFloatOrMulInt
{
    static T Do(T a, T b)
    {
        return T(0);
    }
};

template <> 
class AddFloatOrMulInt<int>
{
public:
    static int Do(int a, int b) // 
    {
        return a * b;
    }
};

template <> 
class AddFloatOrMulInt<float>
{
public:
    static float Do(float a, float b)
    {
        return a + b;
    }
};
```

``` C++
template <typename T> 
class TypeToID
{
public:
    static int const ID = -1;
};

template <> 
class TypeToID<uint8_t>
{
public:
    static int const ID = 0;
};
```
ID is like a function executed in compile time
``` C++
void PrintID()
{
    cout << "ID of uint8_t: " << TypeToID<uint8_t>::ID << endl;
}
```
#### 5 Special case
``` C++
template <> 
class TypeToID<float>
{
    static int const ID = 0xF10A7;
};

template <> 
class TypeToID<void*>
{
    static int const ID = 0x401d;
};

void PrintID()
{
    cout << "ID of uint8_t: " << TypeToID<void*>::ID << endl;
}
```

``` C++
class ClassB {};

template <> 
class TypeToID<void ()>;  
template <> 
class TypeToID<int[3]>;      
template <> 
class TypeToID<int (int[3])>; 
template <> 
class TypeToID<int (ClassB::*[3])(void*, float[2])>; 
template <> 
class TypeToID<int const * volatile * const volatile>;
```

``` C++
void PrintID()
{
    cout << "ID of double: " << TypeToID<double>::ID << endl;
}
```
The output is -1 because there's no template for double type. 

``` C++
template <typename T> class TypeToID
{
public:
    static int const NotID = -2;
};

template <> class TypeToID<float>
{
public:
    static int const ID = 1;
};

void PrintID()
{
    cout << "ID of float: " << TypeToID<float>::ID << endl;       // Print "1"
    cout << "NotID of float: " << TypeToID<float>::NotID << endl; // Error! no NotID for float type
    cout << "ID of double: " << TypeToID<double>::ID << endl;     // Error! no ID for default type 
}
```
Copy function using template
``` C++
template <typename T>
void copy(T* dst, T const* src, size_t elemCount)
{
    for(size_t i = 0; i < elemCount; ++i)
    {
        dst[i] = src[i];
    }
}
```

``` C++
template <typename T> 
class TypeToID<T*> 
{
public:
 static int const ID = 0x80000000;	
};
```
``` C++
void PrintID()
{
    cout << "ID of float*: " << TypeToID<float*>::ID << endl;
}
```

``` C++
template <typename T> 
class TypeToID<T*> 
{
public:
    typedef T		 SameAsT;
    static int const ID = 0x80000000; 
};

void PrintID()
{
	cout << "ID of float*: " << TypeToID< TypeToID<float*>::SameAsT >::ID << endl;
}
```

``` C++
template <typename T>
class RemovePointer<T*>	
{
public:
    typedef T Result;  
};

void Foo()
{
    RemovePointer<float*>::Result x = 5.0f;
    std::cout << x << std::endl;
}
```

```C++
int a;
struct B { int v; };
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

int main()
{
    x.foo(); // error, s_mem not found
    return 0;
}
```

```C++
template <typename T> struct DoWork;	 // (0) 这是原型

template <> struct DoWork<int> {};       // (1) 这是 int 类型的"重载"
template <> struct DoWork<float> {};     // (2) 这是 float 类型的"重载"
template <> struct DoWork<int, int> {};  // (3) 这是 int, int 类型的“重载”

void f(){
    DoWork<int>      i;
    DoWork<float>    f;
    DoWork<int, int> ii;
}
```
5 : error: too many template arguments for class template 'DoWork'
template <> struct DoWork<int, int> {}; 
^ ~~~~
1 : note: template is declared here
template <typename T> struct DoWork {}; 
~~~~~~~~~~~~~~~~~~~~~ ^
```

Partial initialization

```C++
template <typename A, typename B>
void f(int,B*);
```
```C++
template <typename T> struct DoWork;	      // (0) 这是原型

template <> struct DoWork<int> {};            // (1) 这是 int 类型的特化
template <> struct DoWork<float> {};          // (2) 这是 float 类型的特化
template <typename U> struct DoWork<U*> {};   // (3) 这是指针类型的偏特化

DoWork<int>    i;  // (4)
DoWork<float*> pf; // (5)
```
```C++
{   // 以下为 DoWork<int> 查找对应匹配的伪代码
    templateProtoInt = TemplateDict.find(DoWork, int);    // 查找模板原型，查找到(0)
    template = templatePrototype.match(int);              // 以 int 对应 int 匹配到 (1)
}

{   // 以下为DoWork<float*> 查找对应匹配的伪代码
    templateProtoIntPtr = TemplateDict.find(DoWork, float*) // 查找模板原型，查找到(0)
    template = templateProtoIntPtr.match(float*)            // 以 float* 对应 U* 匹配到 (3)，此时U为float

```

```C++
template <typename T, typename U> struct X            ;    // 0 
template <typename T>             struct X<T,  T  > {};    // 1
template <typename T>             struct X<T*, T  > {};    // 2
template <typename T>             struct X<T,  T* > {};    // 3
template <typename U>             struct X<U,  int> {};    // 4
template <typename U>             struct X<U*, int> {};    // 5
template <typename U, typename T> struct X<U*, T* > {};    // 6
template <typename U, typename T> struct X<U,  T* > {};    // 7
template <typename T>             struct X<unique_ptr<T>, shared_ptr<T>>; // 8

X<float*,  int>      v0;                       
X<double*, int>      v1;                       
X<double,  double>   v2;                          
X<float*,  double*>  v3;                           
X<float*,  float*>   v4;                          
X<double,  float*>   v5;                          
X<int,     double*>  v6;                           
X<int*,    int>      v7;                       
X<double*, double>   v8;
```

```
error: no matching function for call to 'foo'
   foo<int, Y>(5, 5.0); // ???
   ^~~~~~~~~~~
   note: candidate template ignored: substitution failure [with T = int, U = Y]: no type named 'type' in 'Y'
       void foo(T t, typename U::type u) {
```

```C++
struct X {
  typedef float type;
};

struct Y {
  typedef float type2;
};

template <typename T, typename U>
void foo(T t, typename U::type u) {
  // ...
}

template <typename T, typename U>
void foo(T t, typename U::type2 u) {
  // ...
} 
void callFoo() {
  foo<int, X>(5, 5.0); // T == int, typename U::type == X::type == float
  foo<int, Y>( 1, 1.0 ); // ???
}
```

所有标记为 `1` 的部分，都是需要替换的部分，而它们在替换过程中的失败（failure），就称之为替换失败（substitution failure）。

下面的代码是提供了一些替换成功和替换失败的示例：

```C++
struct X {
  typedef int type;
};

struct Y {
  typedef int type2;
};

template <typename T> void foo(typename T::type);    // Foo0
template <typename T> void foo(typename T::type2);   // Foo1
template <typename T> void foo(T);                   // Foo2

void callFoo() {
   foo<X>(5);    // Foo0: Succeed, Foo1: Failed,  Foo2: Failed
   foo<Y>(10);   // Foo0: Failed,  Foo1: Succeed, Foo2: Failed
   foo<int>(15); // Foo0: Failed,  Foo1: Failed,  Foo2: Succeed
}
```
