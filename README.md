# TypeInfo
A header-only library which can provide more feature than std::type_info

### Features
- TypeInfo class: a class that contains information of a type (like std::type_Info)
- GetTypeInfo<T>: get TypeInfo object of a type (constexpr)
- TypeName(const TypeInfo&): get a human-readable name of TypeInfo object (unlike typeid(T).name())
- unlike boost::type_index, GetTypeInfo<T> will not stripe const, volatile and reference from T (i.e. GetTypeInfo<const int> != GetTypeInfo<int>)

### Limitations
- Do not support non-type template (i.e. std::array<T, std::size_t>)
- Do not support function types and template types which have more than 10 arguments (if your function have more than 10 arguments, you should consider rewrite it.)

### Tested On
- clang version 10.0.0
- gcc version 9.3.0
- MSVC v142

### Requirements
- a c++ compiler that support constexpr

### Implement GetTypeInfo<> for your class type:
- Use InstantiateTypeInfo(YourType) for non-template class
- Use InstantiateTemplateTypeInfo(YourType) for template class with one template argument
- Use InstantiateTemplateTypeInfoMultiArgs(YourType) for template class with multiple template arguments

### Example
see [Example.cpp](Example.cpp)
