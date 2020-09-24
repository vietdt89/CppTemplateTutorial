#include <iostream>

using namespace std;

//typename and class has no difference
template <typename T> class classA; // template class statement
template <class T> class classB{    // template class definition
public:
    T var;
};

template <typename T>
class Vector{
public:
    void push(T const &value){
        cout << value << endl;
    }
};

template <typename T0, typename T1> class ClassC
{
    // Class body ...
};


int main()
{
    classB<int> a;
    cout << typeid (a.var).name() << endl; // --> i

    Vector<int> v1;
    Vector<char> v2;
    v1.push(65);                      // --> 65
    v2.push(65);                      // --> A

    ClassC<int, float> c;
    return 0;
}
