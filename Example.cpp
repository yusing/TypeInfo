#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "TypeInfo.hxx"
template <class>
struct MyStruct{};
InstantiateTemplateTypeInfo(MyStruct);

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
    /* const, volatile and reference will not be striped so static_assert should succeed. */
    static_assert(GetTypeInfo<int> != GetTypeInfo<const int>, "");
    static_assert(GetTypeInfo<int> != GetTypeInfo<int&>, "");
    static_assert(GetTypeInfo<int> != GetTypeInfo<int&&>, "");
    static_assert(GetTypeInfo<int&> != GetTypeInfo<int&&>, "");
    static_assert(GetTypeInfo<int*> != GetTypeInfo<int*&>, "");
    static_assert(GetTypeInfo<int*> != GetTypeInfo<int*&&>, "");
    static_assert(GetTypeInfo<int*[]> != GetTypeInfo<int**>, "");
    static_assert(GetTypeInfo<int> != GetTypeInfo<volatile int>, "");
    static_assert(GetTypeInfo<volatile int> != GetTypeInfo<volatile const int>, "");

    cout << TypeName(GetTypeInfo<int>) << endl; // output: "int"
    cout << TypeName(GetTypeInfo<int*>) << endl; // output: "int*"
    cout << TypeName(GetTypeInfo<int const*>) << endl; // output: "const int*"
    cout << TypeName(GetTypeInfo<const int*>) << endl; // output: "const int*"
    cout << TypeName(GetTypeInfo<const int*&>) << endl; // output: "const int*&"
    cout << TypeName(GetTypeInfo<const int**&>) << endl; // output: "const int**&"
    cout << TypeName(GetTypeInfo<const int&&>) << endl; // output: "const int&&"
    cout << TypeName(GetTypeInfo<const int**&&>) << endl; // output: "const int**&&"
    cout << TypeName(GetTypeInfo<volatile int*>) << endl; // output: "volatile int*"
    cout << TypeName(GetTypeInfo<int const volatile*>) << endl; // output: "const volatile int*"
    cout << TypeName(GetTypeInfo<int const volatile*[]>) << endl; // output: "const volatile int*[]"
    cout << TypeName(GetTypeInfo<double(*)()>) << endl; // output: "double(*)(void)"
    cout << TypeName(GetTypeInfo<double(*)(int,int,int)>) << endl; // output: "double(*)(int, int, int)"
    cout << TypeName(GetTypeInfo<MyStruct<MyStruct<int*>&&>*&>) << endl; // output: "MyStruct<MyStruct<int*>&&>*&"
    cout << TypeName(GetTypeInfo<MyStruct<int>&(*)(const MyStruct<int>&)>) << endl; // output: "MyStruct<int>&(*)(const MyStruct<int>&)"
    cout << TypeName(GetTypeInfo<const MyStruct<int>&&(*)(MyStruct<int>&&)>) << endl; // output: "const MyStruct<int>&&(*)(MyStruct<int>&&)"
    cout << TypeName(GetTypeInfo<string>) << endl; // output: "std::basic_string<char, std::char_traits<char>, std::allocator<char>>"
    cout << TypeName(GetTypeInfo<decltype(cin)>) << endl; // output: "std::basic_istream<char, std::char_traits<char>>"
    cout << TypeName(GetTypeInfo<decltype(cout)>) << endl; // output: "std::basic_ostream<char, std::char_traits<char>>"
    cout << TypeName(GetTypeInfo<ifstream>) << endl; // output: "std::basic_ifstream<char, std::char_traits<char>>"
    cout << TypeName(GetTypeInfo<ofstream>) << endl; // output: "std::basic_ofstream<char, std::char_traits<char>>"
    cout << TypeName(GetTypeInfo<unordered_map<unsigned int, const string&>>) << endl; // output: "std::unordered_map<unsigned int, const std::basic_string<char, std::char_traits<char>, std::allocator<char>>&, std::hash<unsigned int>, std::equal_to<unsigned int>, std::allocator<std::pair<const unsigned int, const std::basic_string<char, std::char_traits<char>, std::allocator<char>>&>>>"
    cout << TypeName(GetTypeInfo<tuple<int, unsigned, long, float, double>>) << endl; // output: "std::tuple<int, unsigned int, long, float, double>"
    cout << TypeName(GetTypeInfo<size_t(*)(const vector<string>&)>) << endl; // output: "unsigned long long(*)(const std::vector<std::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char>>>>&)"
}