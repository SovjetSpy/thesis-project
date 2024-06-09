#pragma once
#include <string>

#include <X11/Xlib.h>
extern "C" {
#include <X11/Xutil.h>
}

#include <algorithm>
#include <array>
#include <cstddef>
#include <string_view>
#include <cstdint>

std::string XRequestCodeToString(unsigned char request_code);

struct point {
    int x, y;
};



namespace impl
{
    template <typename T>
    [[nodiscard]] constexpr std::string_view RawTypeName()
    {
        #ifndef _MSC_VER
        return __PRETTY_FUNCTION__;
        #else
        return __FUNCSIG__;
        #endif
    }

    struct TypeNameFormat
    {
        std::size_t junk_leading = 0;
        std::size_t junk_total = 0;
    };

    constexpr TypeNameFormat type_name_format = []{
        TypeNameFormat ret;
        std::string_view sample = RawTypeName<int>();
        ret.junk_leading = sample.find("int");
        ret.junk_total = sample.size() - 3;
        return ret;
    }();
    static_assert(type_name_format.junk_leading != std::size_t(-1), "Unable to determine the type name format on this compiler.");

    template <typename T>
    static constexpr auto type_name_storage = []{
        std::array<char, RawTypeName<T>().size() - type_name_format.junk_total + 1> ret{};
        std::copy_n(RawTypeName<T>().data() + type_name_format.junk_leading, ret.size() - 1, ret.data());
        return ret;
    }();
}

template <typename T>
[[nodiscard]] constexpr std::string_view TypeName()
{
    return {impl::type_name_storage<T>.data(), impl::type_name_storage<T>.size() - 1};
}

template <typename T>
[[nodiscard]] constexpr const char *TypeNameCstr()
{
    return impl::type_name_storage<T>.data();
}

Atom make_atom(const char* name, bool if_exists);

template<class T>
struct base_type { using type = T; };

#define DEF_CLASS_WRAPPER(type_name) \
struct type_name##_wrapper { \
    private: \
        type_name val; \
    public: \
        type_name##_wrapper(type_name val = 0) : val(val) {} \
        operator type_name &() { return val; } \
        type_name* operator &() { return &val; } \
}; \
template<> \
struct base_type<type_name##_wrapper> { using type = type_name; };



DEF_CLASS_WRAPPER(Window)
DEF_CLASS_WRAPPER(Atom)
DEF_CLASS_WRAPPER(int32_t)
DEF_CLASS_WRAPPER(uint32_t)
DEF_CLASS_WRAPPER(int16_t)
DEF_CLASS_WRAPPER(uint16_t)
DEF_CLASS_WRAPPER(int8_t)
DEF_CLASS_WRAPPER(uint8_t)
DEF_CLASS_WRAPPER(int64_t)
DEF_CLASS_WRAPPER(uint64_t)
DEF_CLASS_WRAPPER(float)
DEF_CLASS_WRAPPER(double)
DEF_CLASS_WRAPPER(char)
DEF_CLASS_WRAPPER(bool)