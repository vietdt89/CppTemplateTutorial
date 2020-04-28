
# C++ Template Advance guide

Appropriate readership
This article is not used to get started with C++, the example involves some other knowledge. So it will be easier if the reader is:
+ Familiar with C++
+ Get used to STL
+ Familiar with algorithms
This article is not just a repetition of ["C++ template"](https://www.amazon.com/C-Templates-Complete-Guide-2nd/dp/0321714121) and less overlap with ["Modern C++ Design"](https://www.amazon.com/Modern-Design-Programming-Patterns-Depth-ebook/dp/B00AU3JUHG/ref=pd_vtpd_14_6/142-2615446-3327350)
It is suggested that you read the article first, then read"C++ template" to get richer syntax and implementation details, then go further "Modern C++ design" in addition to meta programming. 
Copyright: This article belongs to @wuye9036. I translate and modify what I consider important

## 1. Template basic syntax

### 1.1 Template Class basic syntax

#### 1.1.1 Template Class and member variable definition

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
#### 1.1.3 Template class function definition
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
 static int const ID = 0x80000000;	// 用最高位表示它是一个指针
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

```C++
struct A;
template <typename T> struct B;
template <typename T> struct X {
    typedef X<T> _A; // 编译器当然知道 X<T> 是一个类型。
    typedef X    _B; // X 等价于 X<T> 的缩写
    typedef T    _C; // T 不是一个类型还玩毛
    
    // ！！！注意我要变形了！！！
    class Y {
        typedef X<T>     _D;          // X 的内部，既然外部高枕无忧，内部更不用说了
        typedef X<T>::Y  _E;          // 嗯，这里也没问题，编译器知道Y就是当前的类型，
                                      // 这里在VS2015上会有错，需要添加 typename，
                                      // Clang 上顺利通过。
        typedef typename X<T*>::Y _F; // 这个居然要加 typename！
                                      // 因为，X<T*>和X<T>不一样哦，
                                      // 它可能会在实例化的时候被别的偏特化给抢过去实现了。
    };
    
    typedef A _G;                   // 嗯，没问题，A在外面声明啦
    typedef B<T> _H;                // B<T>也是一个类型
    typedef typename B<T>::type _I; // 嗯，因为不知道B<T>::type的信息，
                                    // 所以需要typename
    typedef B<int>::type _J;        // B<int> 不依赖模板参数，
                                    // 所以编译器直接就实例化（instantiate）了
                                    // 但是这个时候，B并没有被实现，所以就出错了
};
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

在这个例子中，我们展现了函数重载可以在两种条件下工作：参数数量相同、类型不同；参数数量不同。

仿照重载的形式，我们通过特化机制，试图实现一个模板的“重载”：

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
template <> struct DoWork<int, int> {}; // 这是 int, int 类型的“重载”
^ ~~~~
1 : note: template is declared here
template <typename T> struct DoWork {}; // 这是原型
~~~~~~~~~~~~~~~~~~~~~ ^
```

从编译出错的失望中冷静一下，在仔细看看函数特化/偏特化和一般模板的不同之处：

```C++
template <typename T> class X      {};
template <typename T> class X <T*> {};
//                            ^^^^ 注意这里
```

```C++
template <typename T> struct DoWork;	      // (0) 这是原型

template <> struct DoWork<int> {};            // (1) 这是 int 类型的特化
template <> struct DoWork<float> {};          // (2) 这是 float 类型的特化
template <typename U> struct DoWork<U*> {};   // (3) 这是指针类型的偏特化

DoWork<int>    i;  // (4)
DoWork<float*> pf; // (5)
```

首先，编译器分析(0), (1), (2)三句，得知(0)是模板的原型，(1)，(2)，(3)是模板(0)的特化或偏特化。我们假设有两个字典，第一个字典存储了模板原型，我们称之为`TemplateDict`。第二个字典`TemplateSpecDict`，存储了模板原型所对应的特化/偏特化形式。所以编译器在处理这几句时，可以视作

```C++
// 以下为伪代码
TemplateDict[DoWork<T>] = {
    DoWork<int>,
    DoWork<float>,
    DoWork<U*>                     
};
```

然后 (4) 试图以`int`实例化类模板`DoWork`。它会在`TemplateDict`中，找到`DoWork`，它有一个形式参数`T`接受类型，正好和我们实例化的要求相符合。并且此时`T`被推导为`int`。(5) 中的`float*`也是同理。

```C++
{   // 以下为 DoWork<int> 查找对应匹配的伪代码
    templateProtoInt = TemplateDict.find(DoWork, int);    // 查找模板原型，查找到(0)
    template = templatePrototype.match(int);              // 以 int 对应 int 匹配到 (1)
}

{   // 以下为DoWork<float*> 查找对应匹配的伪代码
    templateProtoIntPtr = TemplateDict.find(DoWork, float*) // 查找模板原型，查找到(0)
    template = templateProtoIntPtr.match(float*)            // 以 float* 对应 U* 匹配到 (3)，此时U为float
}
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

在上面这段例子中，有几个值得注意之处。首先，偏特化时的模板形参，和原型的模板形参没有任何关系。和原型不同，它的顺序完全不影响模式匹配的顺序，它只是偏特化模式，如`<U, int>`中`U`的声明，真正的模式，是由`<U, int>`体现出来的。

这也是为什么在特化的时候，当所有类型都已经确定，我们就可以抛弃全部的模板参数，写出`template <> struct X<int, float>`这样的形式：因为所有列表中所有参数都确定了，就不需要额外的形式参数了。

其次，作为一个模式匹配，偏特化的实参列表中展现出来的“样子”，就是它能被匹配的原因。比如，`struct X<T, T>`中，要求模板的两个参数必须是相同的类型。而`struct X<T, T*>`，则代表第二个模板类型参数必须是第一个模板类型参数的指针，比如`X<float***, float****>`就能匹配上。当然，除了简单的指针、`const`和`volatile`修饰符，其他的类模板也可以作为偏特化时的“模式”出现，例如示例8，它要求传入同一个类型的`unique_ptr`和`shared_ptr`。C++标准中指出下列模式都是可以被匹配的：

> N3337, 14.8.2.5/8

> 令`T`是模板类型实参或者类型列表（如 _int, float, double_  这样的，`TT`是template-template实参（参见6.2节），`i`是模板的非类型参数（整数、指针等），则以下形式的形参都会参与匹配：

> `T`, `cv-list T`, `T*`, `template-name <T>`, `T&`, `T&&`

>`T [ integer-constant ]`

>`type (T)`, `T()`, `T(T)`

>`T type ::*`, `type T::*`, `T T::*`

>`T (type ::*)()`, `type (T::*)()`, `type (type ::*)(T)`, `type (T::*)(T)`, `T (type ::*)(T)`, `T (T::*)()`, `T (T::*)(T)`

>`type [i]`, `template-name <i>`, `TT<T>`, `TT<i>`, `TT<>`

对于某些实例化，偏特化的选择并不是唯一的。比如v4的参数是`<float*, float*>`，能够匹配的就有三条规则，1，6和7。很显然，6还是比7好一些，因为能多匹配一个指针。但是1和6，就很难说清楚谁更好了。一个说明了两者类型相同；另外一个则说明了两者都是指针。所以在这里，编译器也没办法决定使用那个，只好报出了编译器错误。

其他的示例可以先自己推测一下， 再去编译器上尝试一番：[`goo.gl/9UVzje`](http://goo.gl/9UVzje)。

#### 3.1.2 不定长的模板参数

不过这个时候也许你还不死心。有没有一种办法能够让例子`DoWork`像重载一样，支持对长度不一的参数列表分别偏特化/特化呢？

答案当然是肯定的。

首先，首先我们要让模板实例化时的模板参数统一到相同形式上。逆向思维一下，虽然两个类型参数我们很难缩成一个参数，但是我们可以通过添加额外的参数，把一个扩展成两个呀。比如这样：

```C++
DoWork<int,   void> i;
DoWork<float, void> f;
DoWork<int,   int > ii;
```

这时，我们就能写出统一的模板原型：

```C++
template <typename T0, typename T1> struct DoWork;
```

继而偏特化/特化问题也解决了：

```C++
template <> struct DoWork<int,   void> {};  // (1) 这是 int 类型的特化
template <> struct DoWork<float, void> {};  // (2) 这是 float 类型的特化
template <> struct DoWork<int,    int> {};  // (3) 这是 int, int 类型的特化
```

显而易见这个解决方案并不那么完美。首先，不管是偏特化还是用户实例化模板的时候，都需要多撰写好几个`void`，而且最长的那个参数越长，需要写的就越多；其次，如果我们的`DoWork`在程序维护的过程中新加入了一个参数列表更长的实例，那么最悲惨的事情就会发生 —— 原型、每一个偏特化、每一个实例化都要追加上`void`以凑齐新出现的实例所需要的参数数量。

所幸模板参数也有一个和函数参数相同的特性：默认实参（Default Arguments）。只需要一个例子，你们就能看明白了[`goo.gl/TtmcY9`](http://goo.gl/TtmcY9)：

``` C++
template <typename T0, typename T1 = void> struct DoWork;

template <typename T> struct DoWork<T> {};
template <>           struct DoWork<int> {};
template <>           struct DoWork<float> {};
template <>           struct DoWork<int, int> {};

DoWork<int> i;
DoWork<float> f;
DoWork<double> d;
DoWork<int, int> ii;
```

所有参数不足，即原型中参数`T1`没有指定的地方，都由T1自己的默认参数`void`补齐了。

但是这个方案仍然有些美中不足之处。

比如，尽管我们默认了所有无效的类型都以`void`结尾，所以正确的类型列表应该是类似于`<int, float, char, void, void>`这样的形态。但你阻止不了你的用户写出类似于`<void, int, void, float, char, void, void>`这样不符合约定的类型参数列表。

其次，假设这段代码中有一个函数，它的参数使用了和类模板相同的参数列表类型，如下面这段代码：

```C++
template <typename T0, typename T1 = void> struct X {
    static void call(T0 const& p0, T1 const& p1);        // 0
};

template <typename T0> struct X<T0> {
    static void call(T0 const& p0);                      // 1
};

void foo(){
    X<int>::call(5);                // 调用函数 1
    X<int, float>::call(5, 0.5f);   // 调用函数 0
}
```

那么，每加一个参数就要多写一个偏特化的形式，甚至还要重复编写一些可以共享的实现。

不过不管怎么说，以长参数加默认参数的方式支持变长参数是可行的做法，这也是C++98/03时代的唯一选择。

例如，[`Boost.Tuple`](https://github.com/boostorg/tuple/blob/develop/include/boost/tuple/detail/tuple_basic.hpp)就使用了这个方法，支持了变长的Tuple：

```C++
// Tuple 的声明，来自 boost
struct null_type;

template <
  class T0 = null_type, class T1 = null_type, class T2 = null_type,
  class T3 = null_type, class T4 = null_type, class T5 = null_type,
  class T6 = null_type, class T7 = null_type, class T8 = null_type,
  class T9 = null_type>
class tuple;

// Tuple的一些用例
tuple<int> a;
tuple<double&, const double&, const double, double*, const double*> b;
tuple<A, int(*)(char, int), B(A::*)(C&), C> c;
tuple<std::string, std::pair<A, B> > d;
tuple<A*, tuple<const A*, const B&, C>, bool, void*> e;
```

此外，Boost.MPL也使用了这个手法将`boost::mpl::vector`映射到`boost::mpl::vector _n_`上。但是我们也看到了，这个方案的缺陷很明显：代码臃肿和潜在的正确性问题。此外，过度使用模板偏特化、大量冗余的类型参数也给编译器带来了沉重的负担。

为了缓解这些问题，在C++11中，引入了变参模板（Variadic Template）。我们来看看支持了变参模板的C++11是如何实现tuple的：

```C++
template <typename... Ts> class tuple;
```

是不是一下子简洁了很多！这里的`typename... Ts`相当于一个声明，是说`Ts`不是一个类型，而是一个不定常的类型列表。同C语言的不定长参数一样，它通常只能放在参数列表的最后。看下面的例子：

```C++
template <typename... Ts, typename U> class X {};              // (1) error!
template <typename... Ts>             class Y {};              // (2)
template <typename... Ts, typename U> class Y<U, Ts...> {};    // (3)
template <typename... Ts, typename U> class Y<Ts..., U> {};    // (4) error!
```

为什么第(1)条语句会出错呢？(1)是模板原型，模板实例化时，要以它为基础和实例化时的类型实参相匹配。因为C++的模板是自左向右匹配的，所以不定长参数只能结尾。其他形式，无论写作`Ts, U`，或者是`Ts, V, Us,`，或者是`V, Ts, Us`都是不可取的。(4) 也存在同样的问题。

但是，为什么(3)中， 模板参数和(1)相同，都是`typename... Ts, typename U`，但是编译器却并没有报错呢？

答案在这一节的早些时候。(3)和(1)不同，它并不是模板的原型，它只是`Y`的一个偏特化。回顾我们在之前所提到的，偏特化时，模板参数列表并不代表匹配顺序，它们只是为偏特化的模式提供的声明，也就是说，它们的匹配顺序，只是按照`<U, Ts...>`来，而之前的参数只是告诉你`Ts`是一个类型列表，而`U`是一个类型，排名不分先后。

在这里，我们只提到了变长模板参数的声明，如何使用我们将在第四章讲述。

#### 3.1.3 模板的默认实参

在上一节中，我们介绍了模板对默认实参的支持。当时我们的例子很简单，默认模板实参是一个确定的类型`void`或者自定义的`null_type`：

```C++
template <
    typename T0, typename T1 = void, typename T2 = void
> class Tuple;
```

实际上，模板的默认参数不仅仅可以是一个确定的类型，它还能是以其他类型为参数的一个类型表达式。
考虑下面的例子：我们要执行两个同类型变量的除法，它对浮点、整数和其他类型分别采取不同的措施。
对于浮点，执行内置除法；对于整数，要处理除零保护，防止引发异常；对于其他类型，执行一个叫做`CustomeDiv`的函数。

第一步，我们先把浮点正确的写出来：

```C++
#include <type_traits>

template <typename T> T CustomDiv(T lhs, T rhs) {
    // Custom Div的实现
}

template <typename T, bool IsFloat = std::is_floating_point<T>::value> struct SafeDivide {
    static T Do(T lhs, T rhs) {
        return CustomDiv(lhs, rhs);
    }
};

template <typename T> struct SafeDivide<T, true>{    // 偏特化A
    static T Do(T lhs, T rhs){
        return lhs/rhs;
    }
};

template <typename T> struct SafeDivide<T, false>{   // 偏特化B
    static T Do(T lhs, T rhs){
        return lhs;
    }
};

void foo(){
    SafeDivide<float>::Do(1.0f, 2.0f);	// 调用偏特化A
    SafeDivide<int>::Do(1, 2);          // 调用偏特化B
}
```

在实例化的时候，尽管我们只为`SafeDivide`指定了参数`T`，但是它的另一个参数`IsFloat`在缺省的情况下，可以根据`T`，求出表达式`std::is_floating_point<T>::value`的值作为实参的值，带入到`SafeDivide`的匹配中。

嗯，这个时候我们要再把整型和其他类型纳入进来，无外乎就是加这么一个参数[`goo.gl/0Lqywt`](http://goo.gl/0Lqywt)：

```C++
#include <complex>
#include <type_traits>

template <typename T> T CustomDiv(T lhs, T rhs) {
    T v;
    // Custom Div的实现
    return v;
}

template <
    typename T,
    bool IsFloat = std::is_floating_point<T>::value,
    bool IsIntegral = std::is_integral<T>::value
> struct SafeDivide {
    static T Do(T lhs, T rhs) {
        return CustomDiv(lhs, rhs);
    }
};

template <typename T> struct SafeDivide<T, true, false>{    // 偏特化A
    static T Do(T lhs, T rhs){
        return lhs/rhs;
    }
};

template <typename T> struct SafeDivide<T, false, true>{   // 偏特化B
    static T Do(T lhs, T rhs){
        return rhs == 0 ? 0 : lhs/rhs;
    }
};

void foo(){
    SafeDivide<float>::Do(1.0f, 2.0f);	                          // 调用偏特化A
    SafeDivide<int>::Do(1, 2);                                    // 调用偏特化B
    SafeDivide<std::complex<float>>::Do({1.f, 2.f}, {1.f, -2.f}); // 调用一般形式
}
```

当然，这时也许你会注意到，`is_integral`，`is_floating_point`和其他类类型三者是互斥的，那能不能只使用一个条件量来进行分派呢？答案当然是可以的：[`goo.gl/jYp5J2`](http://goo.gl/jYp5J2)：

```cpp
#include <complex>
#include <type_traits>

template <typename T> T CustomDiv(T lhs, T rhs) {
    T v;
    // Custom Div的实现
    return v;
}

template <typename T, typename Enabled = std::true_type> struct SafeDivide {
    static T Do(T lhs, T rhs) {
        return CustomDiv(lhs, rhs);
    }
};

template <typename T> struct SafeDivide<
    T, typename std::is_floating_point<T>::type>{    // 偏特化A
    static T Do(T lhs, T rhs){
        return lhs/rhs;
    }
};

template <typename T> struct SafeDivide<
    T, typename std::is_integral<T>::type>{          // 偏特化B
    static T Do(T lhs, T rhs){
        return rhs == 0 ? 0 : lhs/rhs;
    }
};

void foo(){
    SafeDivide<float>::Do(1.0f, 2.0f);	// 调用偏特化A
    SafeDivide<int>::Do(1, 2);          // 调用偏特化B
    SafeDivide<std::complex<float>>::Do({1.f, 2.f}, {1.f, -2.f});
}
```

我们借助这个例子，帮助大家理解一下这个结构是怎么工作的：

1. 对`SafeDivide<int>`

  * 通过匹配类模板的泛化形式，计算默认实参，可以知道我们要匹配的模板实参是`SafeDivide<int, true_type>`
  
  * 计算两个偏特化的形式的匹配：A得到`<int, false_type>`,和B得到 `<int, true_type>`
  
  * 最后偏特化B的匹配结果和模板实参一致，使用它。
  
2. 针对`SafeDivide<complex<float>>`
  
  * 通过匹配类模板的泛化形式，可以知道我们要匹配的模板实参是`SafeDivide<complex<float>, true_type>`
  
  * 计算两个偏特化形式的匹配：A和B均得到`SafeDivide<complex<float>, false_type>`
  
  * A和B都与模板实参无法匹配，所以使用原型，调用`CustomDiv`

### 3.2 后悔药：SFINAE

考虑下面这个函数模板：

``` C++
template <typename T, typename U>
void foo(T t, typename U::type u) {
  // ...
}
```

到本节为止，我们所有的例子都保证了一旦咱们敲定了模板参数中 `T` 和 `U`，函数参变量 `t` 和 `u` 的类型都是成立的，比如下面这样：

``` C++
struct X {
  typedef float type;
};

template <typename T, typename U>
void foo(T t, typename U::type u) {
  // ...
}

void callFoo() {
  foo<int, X>(5, 5.0); // T == int, typename U::type == X::type == float
}
```

那么这里有一个可能都不算是问题的问题 —— 对于下面的代码，你认为它会提示怎么样的错误：

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

void callFoo() {
  foo<int, X>(5, 5.0); // T == int, typename U::type == X::type == float
  foo<int, Y>(5, 5.0); // ???
}
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

在这个例子中，当我们指定 `foo<Y>` 的时候，substitution就开始工作了，而且会同时工作在三个不同的 `foo` 签名上。如果我们仅仅因为 `Y` 没有 `type`，匹配 `Foo0` 失败了，就宣布代码有错，中止编译，那显然是武断的。因为 `Foo1` 是可以被正确替换的，我们也希望 `Foo1` 成为 `foo<Y>` 的原型。

std/boost库中的 `enable_if` 是 SFINAE 最直接也是最主要的应用。所以我们通过下面 `enable_if` 的例子，来深入理解一下 SFINAE 在模板编程中的作用。

假设我们有两个不同类型的计数器（counter），一种是普通的整数类型，另外一种是一个复杂对象，它从接口 `ICounter` 继承，这个接口有一个成员叫做increase实现计数功能。现在，我们想把这两种类型的counter封装一个统一的调用：inc_counter。那么，我们直觉会简单粗暴的写出下面的代码：

```C++
struct ICounter {
  virtual void increase() = 0;
  virtual ~ICounter() {}
};

struct Counter: public ICounter {
   void increase() override {
      // Implements
   }
};

template <typename T>
void inc_counter(T& counterObj) {
  counterObj.increase();
}

template <typename T>
void inc_counter(T& intTypeCounter){
  ++intTypeCounter;
}

void doSomething() {
  Counter cntObj;
  uint32_t cntUI32;

  // blah blah blah
  inc_counter(cntObj);
  inc_counter(cntUI32);
}
```

我们非常希望它展现出预期的行为。因为其实我们是知道对于任何一个调用，两个 `inc_counter` 只有一个是能够编译正确的。“有且唯一”，我们理应当期望编译器能够挑出那个唯一来。

可惜编译器做不到这一点。首先，它就告诉我们，这两个签名

```C++
template <typename T> void inc_counter(T& counterObj);
template <typename T> void inc_counter(T& intTypeCounter);
```

其实是一模一样的。我们遇到了 `redefinition`。

我们看看 `enable_if` 是怎么解决这个问题的。我们通过 `enable_if` 这个 `T` 对于不同的实例做个限定：

```C++
template <typename T> void inc_counter(
  T& counterObj, 
  typename std::enable_if<
    std::is_base_of<ICounter, T>::value
  >::type* = nullptr );

template <typename T> void inc_counter(
  T& counterInt,
  typename std::enable_if<
    std::is_integral<T>::value
  >::type* = nullptr );
```

然后我们解释一下，这个 `enable_if` 是怎么工作的，语法为什么这么丑：

首先，替换（substitution）只有在推断函数类型的时候，才会起作用。推断函数类型需要参数的类型，所以， `typename std::enable_if<std::is_integral<T>::value>::type` 这么一长串代码，就是为了让 `enable_if` 参与到函数类型中；

其次， `is_integral<T>::value` 返回一个布尔类型的编译器常数，告诉我们它是或者不是一个 `integral type`，`enable_if<C>` 的作用就是，如果这个 `C` 值为 `True`，那么 `enable_if<C>::type` 就会被推断成一个 `void` 或者是别的什么类型，让整个函数匹配后的类型变成 `void inc_counter<int>(int & counterInt, void* dummy = nullptr);` 如果这个值为 `False` ，那么 `enable_if<false>` 这个特化形式中，压根就没有这个 `::type`，于是替换就失败了。和我们之前的例子中一样，这个函数原型就不会被产生出来。

所以我们能保证，无论对于 `int` 还是 `counter` 类型的实例，我们都只有一个函数原型通过了substitution —— 这样就保证了它的“有且唯一”，编译器也不会因为你某个替换失败而无视成功的那个实例。

这个例子说到了这里，熟悉C++的你，一定会站出来说我们只要把第一个签名改成：

```C++
void inc_counter(ICounter& counterObj);
```

就能完美解决这个问题了，根本不需要这么复杂的编译器机制。

嗯，你说的没错，在这里这个特性一点都没用。

这也提醒我们，当你觉得需要写 `enable_if` 的时候，首先要考虑到以下可能性：

  * 重载（对模板函数）
  
  * 偏特化（对模板类而言）
  
  * 虚函数
   
  
但是问题到了这里并没有结束。因为 `increase` 毕竟是个虚函数。假如 `Counter` 需要调用的地方实在是太多了，这个时候我们会非常期望 `increase` 不再是个虚函数以提高性能。此时我们会调整继承层级：

```C++
struct ICounter {};
struct Counter: public ICounter {
  void increase() {
    // impl
  }
};
```

那么原有的 `void inc_counter(ICounter& counterObj)` 就无法再执行下去了。这个时候你可能会考虑一些变通的办法：

```C++
template <typename T>
void inc_counter(ICounter& c) {};

template <typename T>
void inc_counter(T& c) { ++c; };

void doSomething() {
  Counter cntObj;
  uint32_t cntUI32;

  // blah blah blah
  inc_counter(cntObj); // 1
  inc_counter(static_cast<ICounter&>(cntObj)); // 2
  inc_counter(cntUI32); // 3
}
```

对于调用 `1`，因为 `cntObj` 到 `ICounter` 是需要类型转换的，所以比 `void inc_counter(T&) [T = Counter]` 要更差一些。然后它会直接实例化后者，结果实现变成了 `++cntObj`，BOOM！

那么我们做 `2` 试试看？嗯，工作的很好。但是等等，我们的初衷是什么来着？不就是让 `inc_counter` 对不同的计数器类型透明吗？这不是又一夜回到解放前了？

所以这个时候，就能看到 `enable_if` 是如何通过 SFINAE 发挥威力的了：


```C++
#include <type_traits>
#include <utility>
#include <cstdint>

struct ICounter {};
struct Counter: public ICounter {
  void increase() {
    // impl
  }
};

template <typename T> void inc_counter(
  T& counterObj, 
  typename std::enable_if<
    std::is_base_of<ICounter, T>::value
  >::type* = nullptr ){
  counterObj.increase();  
}

template <typename T> void inc_counter(
  T& counterInt,
  typename std::enable_if<
    std::is_integral<T>::value
  >::type* = nullptr ){
  ++counterInt;
}
  
void doSomething() {
  Counter cntObj;
  uint32_t cntUI32;

  // blah blah blah
  inc_counter(cntObj); // OK!
  inc_counter(cntUI32); // OK!
}
```

这个代码是不是看起来有点脏脏的。眼尖的你定睛一瞧，咦， `ICounter` 不是已经空了吗，为什么我们还要用它作为基类呢？

这是个好问题。在本例中，我们用它来区分一个`counter`是不是继承自`ICounter`。最终目的，是希望知道 `counter` 有没有 `increase` 这个函数。

所以 `ICounter` 只是相当于一个标签。而于情于理这个标签都是个累赘。但是在C++11之前，我们并没有办法去写类似于：

```C++
template <typename T> void foo(T& c, decltype(c.increase())* = nullptr);
```

这样的函数签名，因为假如 `T` 是 `int`，那么 `c.increase()` 这个函数调用就不存在。但它又不属于Type Failure，而是一个Expression Failure，在C++11之前它会直接导致编译器出错，这并不是我们所期望的。所以我们才退而求其次，用一个类似于标签的形式来提供我们所需要的类型信息。以后的章节，后面我们会说到，这种和类型有关的信息我们可以称之为 `type traits`。

到了C++11，它正式提供了 Expression SFINAE，这时我们就能抛开 `ICounter` 这个无用的Tag，直接写出我们要写的东西：

```C++
struct Counter {
   void increase() {
      // Implements
   }
};

template <typename T>
void inc_counter(T& intTypeCounter, std::decay_t<decltype(++intTypeCounter)>* = nullptr) {
  ++intTypeCounter;
}

template <typename T>
void inc_counter(T& counterObj, std::decay_t<decltype(counterObj.increase())>* = nullptr) {
  counterObj.increase();
}

void doSomething() {
  Counter cntObj;
  uint32_t cntUI32;

  // blah blah blah
  inc_counter(cntObj);
  inc_counter(cntUI32);
}
```

此外，还有一种情况只能使用 SFINAE，而无法使用包括继承、重载在内的任何方法，这就是Universal Reference。比如，

```C++
// 这里的a是个通用引用，可以准确的处理左右值引用的问题。
template <typename ArgT> void foo(ArgT&& a);
```

假如我们要限定ArgT只能是 float 的衍生类型，那么写成下面这个样子是不对的，它实际上只能接受 float 的右值引用。

```C++
void foo(float&& a);
```

此时的唯一选择，就是使用Universal Reference，并增加 `enable_if` 限定类型，如下面这样：

```C++
template <typename ArgT>
void foo(
  ArgT&& a, 
  typename std::enabled_if<
    std::is_same<std::decay_t<ArgT>, float>::value
  >::type* = nullptr
);
```

从上面这些例子可以看到，SFINAE最主要的作用，是保证编译器在泛型函数、偏特化、及一般重载函数中遴选函数原型的候选列表时不被打断。除此之外，它还有一个很重要的元编程作用就是实现部分的编译期自省和反射。

虽然它写起来并不直观，但是对于既没有编译器自省、也没有Concept的C++11来说，已经是最好的选择了。

（补充例子：构造函数上的enable_if）

## 4 元编程下的数据结构与算法
### 4.1 表达式与数值计算
### 4.1 获得类型的属性——类型萃取（Type Traits） 
### 4.2 列表与数组
### 4.3 字典结构
### 4.4 “快速”排序
### 4.5 其它常用的“轮子”

## 5 模板的进阶技巧
### 5.1 嵌入类
### 5.2 Template-Template Class
### 5.3 高阶函数
### 5.4 闭包：模板的“基于对象”
stl allocator?
mpl::apply
### 5.5 占位符(placeholder)：在C++中实现方言的基石
### 5.6 编译期“多态”

## 6   模板的威力：从foreach, transform到Linq
### 6.1 Foreach与Transform
### 6.2 Boost中的模板
Any Spirit Hana TypeErasure
### 6.3 Reactor、Linq与C++中的实践
### 6.4 更高更快更强：从Linq到FP

## 7   结语：讨论有益，争端无用
### 7.1 更好的编译器，更友善的出错信息
### 7.2 模板的症结：易于实现，难于完美
### 7.3 一些期望
alexandrescu 关于 min max 的讨论：《再谈Min和Max》
std::experimental::any / boost.any 对于 reference 的处理

  [1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf
  
  [2]: http://blog.llvm.org/2009/12/dreaded-two-phase-name-lookup.html
  
  [3]: https://goo.gl/zCRNYx
