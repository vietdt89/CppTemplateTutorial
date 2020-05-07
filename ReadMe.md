
# C++ Template Advance guide

Appropriate readership
This article is not used to get started with C++, the example involves some other knowledge. So it will be easier if the reader is:
+ Familiar with C++
+ Get used to STL
+ Familiar with algorithms
This article is not just a repetition of ["C++ template"](https://www.amazon.com/C-Templates-Complete-Guide-2nd/dp/0321714121) and less overlap with ["Modern C++ Design"](https://www.amazon.com/Modern-Design-Programming-Patterns-Depth-ebook/dp/B00AU3JUHG/ref=pd_vtpd_14_6/142-2615446-3327350)
It is suggested that you read the article first, then read"C++ template" to get richer syntax and implementation details, then go further "Modern C++ design" in addition to meta programming. 

## 1. Template basic syntax
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
``` C++
void foo(int a);
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
### 1.2 Basic Template Function

#### 1.2.1 Template Function declaration and definition

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
#### 1.2.2 Template function usage

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
### 1.3 Template with int type
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

## 2.  Basic meta programming
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
More common problem
``` C++
#define BIN_OP(type, a, op, b, result) (*(type *)(result)) = (*(type const *)(a)) op (*(type const*)(b))
void doDiv(void* out, void const* data0, void const* data1, DATA_TYPE type)
{
    if(type == TYPE_INT)
    {
        BIN_OP(int, data0, *, data1, out);
    }
    else
    {
        BIN_OP(float, data0, +, data1, out);
    }
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

void foo()
{}
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
#### 2.2.3 Special case
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
### 2.3 即用即推导
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
## 3   深入理解特化与偏特化

### 3.1 正确的理解偏特化

#### 3.1.1 偏特化与函数重载的比较

```C++
void doWork(int);
void doWork(float);
void doWork(int, int);

void f() {
    doWork(0);
    doWork(0.5f);
    doWork(0, 0);
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

那么根据上面的步骤所展现的基本原理，我们随便来几个练习：

```C++
template <typename T, typename U> struct X            ;    // 0 
                                                           // 原型有两个类型参数
                                                           // 所以下面的这些偏特化的实参列表
                                                           // 也需要两个类型参数对应
template <typename T>             struct X<T,  T  > {};    // 1
template <typename T>             struct X<T*, T  > {};    // 2
template <typename T>             struct X<T,  T* > {};    // 3
template <typename U>             struct X<U,  int> {};    // 4
template <typename U>             struct X<U*, int> {};    // 5
template <typename U, typename T> struct X<U*, T* > {};    // 6
template <typename U, typename T> struct X<U,  T* > {};    // 7

template <typename T>             struct X<unique_ptr<T>, shared_ptr<T>>; // 8

// 以下特化，分别对应哪个偏特化的实例？
// 此时偏特化中的T或U分别是什么类型？

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

这个时候你也许会说：啊，这个简单，`Y` 没有 `type` 这个成员自然会出错啦！嗯，这个时候咱们来看看Clang给出的结果：

```
error: no matching function for call to 'foo'
   foo<int, Y>(5, 5.0); // ???
   ^~~~~~~~~~~
   note: candidate template ignored: substitution failure [with T = int, U = Y]: no type named 'type' in 'Y'
       void foo(T t, typename U::type u) {
```

完整翻译过来就是，直接的出错原因是没有匹配的 `foo` 函数，间接原因是尝试用 `[T = int, U = y]` 做类型替换的时候失败了，所以这个函数模板就被忽略了。等等，不是出错，而是被忽略了？那么也就是说，只要有别的能匹配的类型兜着，编译器就无视这里的失败了？

银河火箭队的阿喵说，就是这样。不信邪的朋友可以试试下面的代码：

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

这下相信编译器真的是不关心替换失败了吧。我们管这种只要有正确的候选，就无视替换失败的做法为SFINAE。

我们不用纠结这个词的发音，它来自于 Substitution failure is not an error 的首字母缩写。这一句之乎者也般难懂的话，由之乎者 —— 啊，不，Substitution，Failure和Error三个词构成。

我们从最简单的词“Error”开始理解。Error就是一般意义上的编译错误。一旦出现编译错误，大家都知道，编译器就会中止编译，并且停止接下来的代码生成和链接等后续活动。

其次，我们再说“Failure”。很多时候光看字面意思，很多人会把 Failure 和 Error 等同起来。但是实际上Failure很多场合下只是一个中性词。比如我们看下面这个虚构的例子就知道这两者的区别了。

假设我们有一个语法分析器，其中某一个规则需要匹配一个token，它可以是标识符，字面量或者是字符串，那么我们会有下面的代码：

```C++
switch(token)
{
case IDENTIFIER:
    // do something
    break;
case LITERAL_NUMBER:
    // do something
    break;
case LITERAL_STRING:
    // do something
    break;
default:
    throw WrongToken(token);
}
```
假如我们当前的token是 `LITERAL_STRING` 的时候，那么第一步它在匹配 `IDENTIFIER` 时，我们可以认为它失败（failure）了，但是它在第三步就会匹配上，所以它并不是一个错误。

但是如果这个token既不是标识符、也不是数字字面量、也不是字符串字面量，而且我们的语法规定除了这三类值以外其他统统都是非法的时，我们才认为它是一个error。

大家所熟知的函数重载也是如此。比如说下面这个例子：

```C++
struct A {};
struct B: public A {};
struct C {};

void foo(A const&) {}
void foo(B const&) {}

void callFoo() {
  foo( A() );
  foo( B() );
  foo( C() );
}
```

那么 `foo( A() )` 虽然匹配 `foo(B const&)` 会失败，但是它起码能匹配 `foo(A const&)`，所以它是正确的；`foo( B() )` 能同时匹配两个函数原型，但是 `foo(B const&)` 要更好一些，因此它选择了这个原型。而 `foo( C() );` 因为两个函数都匹配失败（Failure）了，所以它找不到相应的原型，这时才会报出一个编译器错误（Error）。

所以到这里我们就明白了，在很多情况下，Failure is not an error。编译器在遇到Failure的时候，往往还需要尝试其他的可能性。

好，现在我们把最后一个词，Substitution，加入到我们的字典中。现在这句话的意思就是说，我们要把 Failure is not an error 的概念，推广到Substitution阶段。

所谓substitution，就是将函数模板中的形参，替换成实参的过程。概念很简洁但是实现却颇多细节，所以C++标准中对这一概念的解释比较拗口。它分别指出了以下几点：

  * 什么时候函数模板会发生实参 替代（Substitute） 形参的行为；
  
  * 什么样的行为被称作 Substitution；
  
  * 什么样的行为不可以被称作 Substitution Failure —— 他们叫SFINAE error。
  
我们在此不再详述，有兴趣的同学可以参照[`这里`](http://en.cppreference.com/w/cpp/language/sfinae)，这是标准的一个精炼版本。这里我们简单的解释一下。

考虑我们有这么个函数签名：

```C++
template <
  typename T0, 
  // 一大坨其他模板参数
  typename U = /* 和前面T有关的一大坨 */
>
RType /* 和模板参数有关的一大坨 */
functionName (
   PType0 /* PType0 是和模板参数有关的一大坨 */,
   PType1 /* PType1 是和模板参数有关的一大坨 */,
   // ... 其他参数
) {
  // 实现，和模板参数有关的一大坨
}
```

那么，在这个函数模板被实例化的时候，所有函数签名上的“和模板参数有关的一大坨”被推导出具体类型的过程，就是替换。一个更具体的例子来解释上面的“一大坨”：

```C++
template <
  typename T, 
  typenname U = typename vector<T>::iterator // 1
>
typename vector<T>::value_type  // 1
  foo( 
      T*, // 1
      T&, // 1
      typename T::internal_type, // 1
      typename add_reference<T>::type, // 1
      int // 这里都不需要 substitution
  )
{
   // 整个实现部分，都没有 substitution。这个很关键。
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
