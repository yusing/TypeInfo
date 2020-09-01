#ifndef TYPE_INFO_HXX
#define TYPE_INFO_HXX

#if __cplusplus >= 201606L || _MSVC_LANG >= 201606L
#define INLINE_VAR inline
#define CXX17_FEATURE(EXPR) EXPR
#else
#define INLINE_VAR
#define CXX17_FEATURE(EXPR)
#endif

#define InstantiateTemplateTypeInfo(CLASS) template <class T> INLINE_VAR constexpr TypeInfo GetTypeInfo<CLASS<T>> = {#CLASS, Qualifier::None, {&GetTypeInfo<T>}}
#define InstantiateTemplateTypeInfoMultiArgs(CLASS) template <class ...T> INLINE_VAR constexpr TypeInfo GetTypeInfo<CLASS<T...>> = {#CLASS, Qualifier::None, (GetArgTypeInfo<T...>)}
#define InstantiateTypeInfo(CLASS) template <> INLINE_VAR constexpr TypeInfo GetTypeInfo<CLASS> = {#CLASS, Qualifier::None};
#include <array>
#include <string>

#ifndef __cpp_char8_t
enum class char8_t : unsigned char {};
#endif

enum class Qualifier: unsigned {
    None = 0u, Const = 1u << 1u, Extent = Const << 1u, Volatile = Extent << 1u, Reference = Volatile << 1u,
    LValueReference = Reference << 1u, ConstLValueRef = Const | LValueReference, ConstRef = Const | Reference,
    ConstExtent = Const | Extent, ConstVolatile = Const | Volatile
};

constexpr Qualifier operator|(Qualifier lhs, Qualifier rhs) {
    return static_cast<Qualifier>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs));
}

constexpr bool operator&&(Qualifier lhs, Qualifier rhs) {
    return static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs);
}

struct TypeInfo;

typedef std::array<TypeInfo const*, 10> ArgsList;

static constexpr char VOID_TYPE[]     = "void";
static constexpr char FUNCTION_TYPE[] = "";
static constexpr char POINTER_TYPE[]  = "";

#pragma pack(push,1)
struct TypeInfo {
    const char*     typeId          = VOID_TYPE; // pointer to static storage
    const Qualifier qualifier;
    TypeInfo const* pReturnTypeInfo = nullptr; // pointer to static storage
    const ArgsList  arrArgTypeInfo{};
    constexpr       TypeInfo(const char* id, const Qualifier q) : typeId{id}, qualifier{q} {}

    constexpr TypeInfo(const char* id, const Qualifier q, const ArgsList& child) : typeId{id}, qualifier{q},
        arrArgTypeInfo(child) {}

    constexpr TypeInfo(const TypeInfo& assign, const Qualifier qMerge) : typeId{assign.typeId}, qualifier{assign.qualifier | qMerge},
                                                                            pReturnTypeInfo{assign.pReturnTypeInfo},
                                                                            arrArgTypeInfo{assign.arrArgTypeInfo} {}

    constexpr TypeInfo(const Qualifier q, const TypeInfo& ret, const ArgsList& args) : typeId{FUNCTION_TYPE},
        qualifier{q}, pReturnTypeInfo{&ret}, arrArgTypeInfo{args} {}

    friend constexpr bool operator==(const TypeInfo& lhs, const TypeInfo& rhs) {
        const auto eq = lhs.typeId == rhs.typeId && lhs.qualifier == rhs.qualifier;
        if (!eq) return false;
        if (lhs.pReturnTypeInfo != rhs.pReturnTypeInfo) return false;
        if (lhs.pReturnTypeInfo != nullptr) return *lhs.pReturnTypeInfo == *rhs.pReturnTypeInfo;
        for (size_t i = 0; i < lhs.arrArgTypeInfo.size(); ++i) {
            if (lhs.arrArgTypeInfo[i] != rhs.arrArgTypeInfo[i]) return false;
            if (lhs.arrArgTypeInfo[i] == nullptr) return eq;
            if (lhs.arrArgTypeInfo[i]->typeId == nullptr) return false;
        }
        return eq;
    }

    friend constexpr bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs) { return !(lhs == rhs); }
};
#pragma pack(pop)

template <class T> struct RemoveAllQualifiers {
    typedef T Type;
};

template <class T> struct RemoveAllQualifiers<T*> : RemoveAllQualifiers<T> {};

template <class T> struct RemoveAllQualifiers<T&> : RemoveAllQualifiers<T> {};

template <class T> struct RemoveAllQualifiers<T&&> : RemoveAllQualifiers<T> {};

template <class T> struct RemoveAllQualifiers<T const> : RemoveAllQualifiers<T> {};

template <class T> struct RemoveAllQualifiers<T volatile> : RemoveAllQualifiers<T> {};

template <class T> struct RemoveAllQualifiers<T const volatile> : RemoveAllQualifiers<T> {};

template <class T> using RemoveAllQualifierType = typename RemoveAllQualifiers<T>::Type;

template <class T> constexpr INLINE_VAR Qualifier GetTypeQualifier             = Qualifier::None;
template <class T> constexpr INLINE_VAR Qualifier GetTypeQualifier<const T>    = Qualifier::Const | GetTypeQualifier<T>;
template <class T> constexpr INLINE_VAR Qualifier GetTypeQualifier<volatile T> = Qualifier::Volatile | GetTypeQualifier<
    T>;
template <class T> constexpr INLINE_VAR Qualifier GetTypeQualifier<T&>  = Qualifier::Reference | GetTypeQualifier<T>;
template <class T> constexpr INLINE_VAR Qualifier GetTypeQualifier<T&&> = Qualifier::LValueReference | GetTypeQualifier<
    T>;
template <class T> constexpr INLINE_VAR Qualifier GetTypeQualifier<T[]> = Qualifier::Extent | GetTypeQualifier<T>;
template <class T, size_t s> constexpr INLINE_VAR Qualifier GetTypeQualifier<T[s]> = Qualifier::Extent |
    GetTypeQualifier<T>;
template <class T> constexpr INLINE_VAR Qualifier GetTypeQualifier<const volatile T> = Qualifier::ConstVolatile |
    GetTypeQualifier<T>;
template <class T> constexpr INLINE_VAR Qualifier GetTypeQualifier<const volatile T[]> = Qualifier::ConstVolatile |
    Qualifier::Extent | GetTypeQualifier<T>;

template <class T> constexpr INLINE_VAR TypeInfo GetTypeInfo = {VOID_TYPE, GetTypeQualifier<T>}; // undefined types
template <> constexpr INLINE_VAR TypeInfo        GetTypeInfo<void> = {VOID_TYPE, Qualifier::None};
template <class T> constexpr INLINE_VAR TypeInfo GetTypeInfo<T&> = {GetTypeInfo<T>, Qualifier::Reference};
template <class T> constexpr INLINE_VAR TypeInfo GetTypeInfo<T*> = {POINTER_TYPE, Qualifier::None, {&GetTypeInfo<T>}};
template <class T> constexpr INLINE_VAR TypeInfo GetTypeInfo<T&&> = {GetTypeInfo<T>, Qualifier::LValueReference};
template <class T> constexpr INLINE_VAR TypeInfo GetTypeInfo<T[]> = {GetTypeInfo<T>, Qualifier::Extent};
template <class T> constexpr INLINE_VAR TypeInfo GetTypeInfo<const T> = {GetTypeInfo<T>, Qualifier::Const};
template <class T> constexpr INLINE_VAR TypeInfo GetTypeInfo<volatile T> = {GetTypeInfo<T>, Qualifier::Volatile};
template <class T> constexpr INLINE_VAR TypeInfo GetTypeInfo<const volatile T> = {GetTypeInfo<T>, Qualifier::ConstVolatile};
template <class T, size_t s> constexpr INLINE_VAR TypeInfo GetTypeInfo<T[s]> = {GetTypeInfo<T>, Qualifier::Extent};
template <class T> constexpr INLINE_VAR TypeInfo GetTypeInfo<T*[]> = {GetTypeInfo<T*>, Qualifier::Extent};

template <class... Args> constexpr INLINE_VAR ArgsList GetArgTypeInfo = {&GetTypeInfo<void>};

template <class Arg> constexpr INLINE_VAR ArgsList              GetArgTypeInfo<Arg>        = {&GetTypeInfo<Arg>};
template <class Arg1, class Arg2> constexpr INLINE_VAR ArgsList GetArgTypeInfo<Arg1, Arg2> = {
    &GetTypeInfo<Arg1>, &GetTypeInfo<Arg2>
};
template <class Arg1, class Arg2, class Arg3> constexpr INLINE_VAR ArgsList GetArgTypeInfo<Arg1, Arg2, Arg3> = {
    &GetTypeInfo<Arg1>, &GetTypeInfo<Arg2>, &GetTypeInfo<Arg3>
};
template <class Arg1, class Arg2, class Arg3, class Arg4> constexpr INLINE_VAR ArgsList GetArgTypeInfo<
    Arg1, Arg2, Arg3, Arg4> = {&GetTypeInfo<Arg1>, &GetTypeInfo<Arg2>, &GetTypeInfo<Arg3>, &GetTypeInfo<Arg4>};
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5> constexpr INLINE_VAR ArgsList GetArgTypeInfo<
    Arg1, Arg2, Arg3, Arg4, Arg5> = {
    &GetTypeInfo<Arg1>, &GetTypeInfo<Arg2>, &GetTypeInfo<Arg3>, &GetTypeInfo<Arg4>, &GetTypeInfo<Arg5>
};
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6> constexpr INLINE_VAR ArgsList
GetArgTypeInfo<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6> = {
    &GetTypeInfo<Arg1>, &GetTypeInfo<Arg2>, &GetTypeInfo<Arg3>, &GetTypeInfo<Arg4>, &GetTypeInfo<Arg5>,
    &GetTypeInfo<Arg6>
};
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7> constexpr INLINE_VAR
ArgsList GetArgTypeInfo<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7> = {
    &GetTypeInfo<Arg1>, &GetTypeInfo<Arg2>, &GetTypeInfo<Arg3>, &GetTypeInfo<Arg4>, &GetTypeInfo<Arg5>,
    &GetTypeInfo<Arg6>, &GetTypeInfo<Arg7>
};
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8> constexpr
INLINE_VAR ArgsList GetArgTypeInfo<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8> = {
    &GetTypeInfo<Arg1>, &GetTypeInfo<Arg2>, &GetTypeInfo<Arg3>, &GetTypeInfo<Arg4>, &GetTypeInfo<Arg5>,
    &GetTypeInfo<Arg6>, &GetTypeInfo<Arg7>, &GetTypeInfo<Arg8>
};
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Arg9
> constexpr INLINE_VAR ArgsList GetArgTypeInfo<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9> = {
    &GetTypeInfo<Arg1>, &GetTypeInfo<Arg2>, &GetTypeInfo<Arg3>, &GetTypeInfo<Arg4>, &GetTypeInfo<Arg5>,
    &GetTypeInfo<Arg6>, &GetTypeInfo<Arg7>, &GetTypeInfo<Arg8>, &GetTypeInfo<Arg9>
};
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Arg9,
          class Arg10> constexpr INLINE_VAR ArgsList GetArgTypeInfo<
    Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10> = {
    &GetTypeInfo<Arg1>, &GetTypeInfo<Arg2>, &GetTypeInfo<Arg3>, &GetTypeInfo<Arg4>, &GetTypeInfo<Arg5>,
    &GetTypeInfo<Arg6>, &GetTypeInfo<Arg7>, &GetTypeInfo<Arg8>, &GetTypeInfo<Arg9>, &GetTypeInfo<Arg10>
};

template <class R, class Arg> constexpr INLINE_VAR TypeInfo GetTypeInfo<R(*)(Arg)> = {
    Qualifier::None, GetTypeInfo<R>, {&GetTypeInfo<Arg>}
};
template <class R, class Arg> constexpr INLINE_VAR TypeInfo GetTypeInfo<const R(*)(Arg)> = {
    Qualifier::Const, GetTypeInfo<R>, {&GetTypeInfo<Arg>}
};
template <class R, class... Args> constexpr INLINE_VAR TypeInfo GetTypeInfo<R(*)(Args ...)> = {
    Qualifier::None, GetTypeInfo<R>, GetArgTypeInfo<Args...>
};
template <class R, class... Args> constexpr INLINE_VAR TypeInfo GetTypeInfo<const R(*)(Args ...)> = {
    Qualifier::Const, GetTypeInfo<R>, GetArgTypeInfo<Args...>
};

inline std::string TypeName(const TypeInfo& info);

inline std::string FormatArgList(const ArgsList& args) {
    if (args[0] == nullptr) return "";
    auto s  = TypeName(*args[0]);
    auto it = args.begin();
    s.reserve(200);
    for (++it; it != args.end(); ++it) {
        const auto& arg = *it;
        if (arg == nullptr || arg->typeId == nullptr) return s;
        s.append(", ");
        s.append(TypeName(*arg));
    }
    return s;
}

inline std::string TypeName(const TypeInfo& info) {
    std::string name;
    const auto  qualifier = info.qualifier;
    name.reserve(200);
    if (qualifier && Qualifier::Const) name.append("const ");
    if (qualifier && Qualifier::Volatile) name.append("volatile ");
    if (info.typeId == FUNCTION_TYPE) {
        name.append(TypeName(*info.pReturnTypeInfo));
        name.append("(*)(");
        name.append(FormatArgList(info.arrArgTypeInfo));
        name.push_back(')');
    } else if (info.typeId == POINTER_TYPE) {
        name.append(FormatArgList(info.arrArgTypeInfo));
        name.push_back('*');
    } else {
        name += info.typeId;
        if (info.arrArgTypeInfo[0] != nullptr) {
            name.push_back('<');
            name.append(FormatArgList(info.arrArgTypeInfo));
            name.push_back('>');
        }
    }

    if (qualifier && Qualifier::Reference) name.push_back('&');
    else { if (qualifier && Qualifier::Extent) name.append("[]"); }
    if (qualifier && Qualifier::LValueReference) name.append(2, '&');
    return name;
}


/* declare all classes in namespace std */
namespace std
{
    /* supported */
    class any;
    template <class> struct char_traits;
    template <class> class allocator;
    template <class> struct hash;
    template <class> struct equal_to;
    template <class> struct optional;
    template <class> struct greater;
    template <class> struct greater_equal;
    template <class> struct less;
    template <class> struct less_equal;
    template <class> struct plus;
    template <class> struct minus;
    template <class> struct multiplies;
    template <class> struct divides;
    template <class> struct modulus;
    template <class> struct negate;
    template <class> struct logical_and;
    template <class> struct logical_or;
    template <class> struct logical_not;
    template <class> struct bit_and;
    template <class> struct bit_or;
    template <class> struct bit_xor;
    template <class> struct bit_not;
    template <class> struct unary_negate;
    template <class> struct binary_negate;
    template <class> class initializer_list;
//    template <class, class, class> class basic_string;
//    template <class, class> class basic_string_view;
    template <class, class> class vector;
    template <class, class> class deque;
    template <class, class> class list;
    template <class, class> class forward_list;
    template <class, class> class queue;
    template <class, class> class stack;
    template <class, class> struct pair;
    template <class, class> class basic_ios;
    template <class, class> class basic_streambuf;
    template <class, class> class basic_istream;
    template <class, class> class basic_ostream;
    template <class, class> class basic_iostream;
    template <class, class> class basic_filebuf;
    template <class, class> class basic_ifstream;
    template <class, class> class basic_ofstream;
    template <class, class> class basic_fstream;
    // template <class, class, class> class basic_stringbuf;
    // template <class, class, class> class basic_istringstream;
    // template <class, class, class> class basic_ostringstream;
    // template <class, class, class> class basic_stringstream;
    template <class...> class tuple;
    template <class...> class variant;
    template <class, class, class> class set;
    template <class, class, class> class multiset;
    template <class, class, class, class> class map;
    template <class, class, class, class> class multimap;
    template <class, class, class> class priority_queue;
    template <class, class, class, class> class unordered_set;
    template <class, class, class, class, class> class unordered_map;
    template <class, class, class, class> class unordered_multiset;
    template <class, class, class, class, class> class unordered_multimap;
    /* non type template (unsupported) */
    template <class, size_t> class array;
}

/* implement TypeInfo for static types */
InstantiateTypeInfo(char);
InstantiateTypeInfo(wchar_t);
InstantiateTypeInfo(char8_t);
InstantiateTypeInfo(char16_t);
InstantiateTypeInfo(char32_t);
InstantiateTypeInfo(signed char);
InstantiateTypeInfo(unsigned char);
InstantiateTypeInfo(int);
InstantiateTypeInfo(long);
InstantiateTypeInfo(long long);
InstantiateTypeInfo(unsigned int);
InstantiateTypeInfo(unsigned long);
InstantiateTypeInfo(unsigned long long);
InstantiateTypeInfo(float);
InstantiateTypeInfo(double);
InstantiateTypeInfo(long double);

/* implement TypeInfo for std:: types */
CXX17_FEATURE(InstantiateTypeInfo(std::byte));
CXX17_FEATURE(InstantiateTypeInfo(std::any));
InstantiateTemplateTypeInfo(std::char_traits);
InstantiateTemplateTypeInfo(std::allocator);
InstantiateTemplateTypeInfo(std::hash);
InstantiateTemplateTypeInfo(std::equal_to);
CXX17_FEATURE(InstantiateTemplateTypeInfo(std::optional));
InstantiateTemplateTypeInfo(std::greater);
InstantiateTemplateTypeInfo(std::greater_equal);
InstantiateTemplateTypeInfo(std::less);
InstantiateTemplateTypeInfo(std::less_equal);
InstantiateTemplateTypeInfo(std::plus);
InstantiateTemplateTypeInfo(std::minus);
InstantiateTemplateTypeInfo(std::multiplies);
InstantiateTemplateTypeInfo(std::divides);
InstantiateTemplateTypeInfo(std::modulus);
InstantiateTemplateTypeInfo(std::negate);
InstantiateTemplateTypeInfo(std::logical_and);
InstantiateTemplateTypeInfo(std::logical_or);
InstantiateTemplateTypeInfo(std::logical_not);
InstantiateTemplateTypeInfo(std::bit_and);
InstantiateTemplateTypeInfo(std::bit_or);
InstantiateTemplateTypeInfo(std::bit_xor);
InstantiateTemplateTypeInfo(std::bit_not);
InstantiateTemplateTypeInfo(std::unary_negate);
InstantiateTemplateTypeInfo(std::binary_negate);
InstantiateTemplateTypeInfo(std::initializer_list);
InstantiateTemplateTypeInfoMultiArgs(std::basic_string);
CXX17_FEATURE(InstantiateTemplateTypeInfoMultiArgs(std::basic_string_view));
InstantiateTemplateTypeInfoMultiArgs(std::vector);
InstantiateTemplateTypeInfoMultiArgs(std::deque);
InstantiateTemplateTypeInfoMultiArgs(std::queue);
InstantiateTemplateTypeInfoMultiArgs(std::list);
InstantiateTemplateTypeInfoMultiArgs(std::forward_list);
InstantiateTemplateTypeInfoMultiArgs(std::stack);
InstantiateTemplateTypeInfoMultiArgs(std::tuple);
InstantiateTemplateTypeInfoMultiArgs(std::pair);
InstantiateTemplateTypeInfoMultiArgs(std::basic_ios);
InstantiateTemplateTypeInfoMultiArgs(std::basic_streambuf);
InstantiateTemplateTypeInfoMultiArgs(std::basic_istream);
InstantiateTemplateTypeInfoMultiArgs(std::basic_ostream);
InstantiateTemplateTypeInfoMultiArgs(std::basic_iostream);
InstantiateTemplateTypeInfoMultiArgs(std::basic_filebuf);
InstantiateTemplateTypeInfoMultiArgs(std::basic_ifstream);
InstantiateTemplateTypeInfoMultiArgs(std::basic_ofstream);
InstantiateTemplateTypeInfoMultiArgs(std::basic_fstream);
InstantiateTemplateTypeInfoMultiArgs(std::basic_stringbuf);
InstantiateTemplateTypeInfoMultiArgs(std::basic_istringstream);
InstantiateTemplateTypeInfoMultiArgs(std::basic_ostringstream);
InstantiateTemplateTypeInfoMultiArgs(std::basic_stringstream);

CXX17_FEATURE(InstantiateTemplateTypeInfoMultiArgs(std::variant));
InstantiateTemplateTypeInfoMultiArgs(std::set);
InstantiateTemplateTypeInfoMultiArgs(std::multiset);
InstantiateTemplateTypeInfoMultiArgs(std::map);
InstantiateTemplateTypeInfoMultiArgs(std::multimap);
InstantiateTemplateTypeInfoMultiArgs(std::priority_queue);
InstantiateTemplateTypeInfoMultiArgs(std::unordered_set);
InstantiateTemplateTypeInfoMultiArgs(std::unordered_map);
InstantiateTemplateTypeInfoMultiArgs(std::unordered_multiset);
InstantiateTemplateTypeInfoMultiArgs(std::unordered_multimap);
#endif
