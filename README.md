# TypeInfo
A header-only library which can provide more feature than std::type_info

### Features
- TypeInfo class: a class that contains information of a type (like std::type_Info)
- GetTypeInfo<T>: get TypeInfo object of a type (constexpr)
- TypeName(const TypeInfo&): get a human-readable name of TypeInfo object (unlike typeid(T).name())

### Limitation
- Do not support non-type template (i.e. std::array<T, std::size_t>)
- Do not support function types and template types which have more than 10 arguments (if your function have more than 10 arguments, you should consider rewrite it.)

### Tested On
- clang version 10.0.0
- gcc version 9.3.0
- MSVC v142

### Requirements
- c++11 or above

### Implement GetTypeInfo<> for your class type:
- Use InstantiateTypeInfo(YourType) for your non-template class
- Use InstantiateTemplateTypeInfo(YourType) for template class with one template argument
- Use InstantiateTemplateTypeInfoMultiArgs(YourType) for template class with multiple template arguments

### Example usage
```c++
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "TypeInfo.hxx"
template <class>
struct Test{};
InstantiateTemplateTypeInfo(Test);

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::unordered_map;
using std::tuple;
using std::vector;

int main()
{
    cout << (GetTypeInfo<int> == GetTypeInfo<const int>) << endl;
    cout << (GetTypeInfo<int> == GetTypeInfo<int&>) << endl;
    cout << (GetTypeInfo<int> == GetTypeInfo<int&&>) << endl;
    cout << (GetTypeInfo<int&> == GetTypeInfo<int&&>) << endl;
    cout << (GetTypeInfo<int*> == GetTypeInfo<int*&>) << endl;
    cout << (GetTypeInfo<int*> == GetTypeInfo<int*&&>) << endl;
    cout << (GetTypeInfo<int*[]> == GetTypeInfo<int**>) << endl;
    cout << (GetTypeInfo<int> == GetTypeInfo<volatile int>) << endl;
    cout << (GetTypeInfo<volatile int> == GetTypeInfo<volatile const int>) << endl;

    cout << TypeName(GetTypeInfo<int>) << endl;
    cout << TypeName(GetTypeInfo<int*>) << endl;
    cout << TypeName(GetTypeInfo<const int*>) << endl;
    cout << TypeName(GetTypeInfo<const int**>) << endl;
    cout << TypeName(GetTypeInfo<const int***>) << endl;
    cout << TypeName(GetTypeInfo<const int*&>) << endl;
    cout << TypeName(GetTypeInfo<const int**&>) << endl;
    cout << TypeName(GetTypeInfo<const int***&>) << endl;
    cout << TypeName(GetTypeInfo<const int&&>) << endl;
    cout << TypeName(GetTypeInfo<const int*&&>) << endl;
    cout << TypeName(GetTypeInfo<const int**&&>) << endl;
    cout << TypeName(GetTypeInfo<volatile int*>) << endl;
    cout << TypeName(GetTypeInfo<int const*>) << endl;
    cout << TypeName(GetTypeInfo<int const volatile*>) << endl;
    cout << TypeName(GetTypeInfo<int const volatile*[]>) << endl;
    cout << TypeName(GetTypeInfo<double(*)()>) << endl;
    cout << TypeName(GetTypeInfo<double(*)(int,int,int)>) << endl;
    cout << TypeName(GetTypeInfo<Test<Test<int*>&&>*&>) << endl;
    cout << TypeName(GetTypeInfo<Test<int>&(*)(const Test<int>&)>) << endl;
    cout << TypeName(GetTypeInfo<Test<int>&(*)(Test<int>&&)>) << endl;
    cout << TypeName(GetTypeInfo<string>) << endl;
    cout << TypeName(GetTypeInfo<decltype(cin)>) << endl;
    cout << TypeName(GetTypeInfo<decltype(cout)>) << endl;
    cout << TypeName(GetTypeInfo<ifstream>) << endl;
    cout << TypeName(GetTypeInfo<ofstream>) << endl;
    cout << TypeName(GetTypeInfo<unordered_map<unsigned int, const string&>>) << endl;
    cout << TypeName(GetTypeInfo<tuple<int, unsigned, long, float, double>>) << endl;
    cout << TypeName(GetTypeInfo<size_t(*)(const vector<string>&)>) << endl;
}
```

Output:
```c++
0
0
0
0
0
0
0
0
0
int
int*
const int*
const int**
const int***
const int*&
const int**&
const int***&
const int&&
const int*&&
const int**&&
volatile int*
const int*
const volatile int*
const volatile int*[]
double(*)(void)
double(*)(int, int, int)
Test<Test<int*>&&>*&
Test<int>&(*)(const Test<int>&)
Test<int>&(*)(Test<int>&&)
std::basic_string<char, std::char_traits<char>, std::allocator<char>>
std::basic_istream<char, std::char_traits<char>>
std::basic_ostream<char, std::char_traits<char>>
std::basic_ifstream<char, std::char_traits<char>>
std::basic_ofstream<char, std::char_traits<char>>
std::unordered_map<unsigned int, const std::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::hash<unsigned int>, std::equal_to<unsigned int>, std::allocator<std::pair<const unsigned int, const std::basic_string<char, std::char_traits<char>, std::allocator<char>>&>>>
std::tuple<int, unsigned int, long, float, double>
unsigned long long(*)(const std::vector<std::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char>>>>&)
```
