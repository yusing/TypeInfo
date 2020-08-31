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