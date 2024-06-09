#pragma once
#include "util.h"
#include <typeinfo>
#include <X11/Xatom.h>
#include <string.h>

template<class T>
class atom;

#define static_CONST(type) static const type


template<class T>
struct atom_type {
    static_assert(std::is_class_v<T>, "atom_type can only be used with classes.");

    using type = base_type<T>::type;
    static_CONST(Atom) type_atom = XA_ATOM;
    static_CONST(int) format = 8;
    static_CONST(int) nitems = sizeof(T);

    static void* get_data(T& data) {
        return &data;
    }

    static void set_data(T* data, void* ret, unsigned long nitems) {
        memcpy(data, ret, nitems);
    }

};

template<>
struct atom_type<int32_t_wrapper> {
    using type = int;
    static_CONST(Atom) type_atom = XA_INTEGER;
    static_CONST(int) format = 8;
    static_CONST(int) nitems = 1;

    static void* get_data(int& data) {
        return &data;
    }

    static void set_data(int* data, void* ret, unsigned long nitems) {
        memcpy(data, ret, nitems);
    }
};

template<>
struct atom_type<Window_wrapper> {
    using type = Window;
    static_CONST(Atom) type_atom = XA_WINDOW;
    static_CONST(int) format = 32;
    static_CONST(int) nitems = 1;

    static void* get_data(Window_wrapper& data) {
        return &data;
    }

    static void set_data(Window_wrapper* data, void* ret, unsigned long nitems) {
        memcpy(data, ret, nitems);
    }
};

template<>
struct atom_type<std::string> {
    using type = std::string;
    static_CONST(Atom) type_atom = XA_STRING;
    static_CONST(int) format = 8;
    static_CONST(int) nitems = 1;

    static void* get_data(std::string& data) {
        return (void*)data.c_str();
    }

    static void set_data(std::string* data, void* ret, unsigned long nitems) {
        *data = std::string((char*)ret);
    }
};

template<class T>
class atom
{
    static_assert(std::is_class_v<T>, "atom_type can only be used with classes.");
    Atom atom_handel;
public:
    using type_information = atom_type<T>;
    atom(const char* name, bool if_exists = false)
    {
        atom_handel = make_atom(name, if_exists);
    }

    Atom get() const {
        return atom_handel;
    }
};

template<>
class atom<void> {
    Atom atom_handel;
public:
    atom(const char* name, bool if_exists = false)
    {
        atom_handel = make_atom(name, if_exists);
    }

    inline Atom get() const {
        return atom_handel;
    }
};

// extern atom<Window> window_handle;

Window get_window_handle(Window w);

void set_window_handle(Window w, Window handle);

// atom<const char*> window_name("_NET_WM_NAME");
// atom<const char*> window_name("WM_NAME");
// atom<Window> window_parent("_NET_WM_PID");
// atom<Window> window_client("WM_CLIENT");
// atom<Window> window_frame("WM_FRAME");
// atom<Window> reporting_window("WM_REPORTING_WINDOW_CA");