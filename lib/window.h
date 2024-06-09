#pragma once
#include "util.h"
#include <vector>
#include "action_handler.h"
#include "atom.h"

class client_window {

    Window client_wn;
    Window frame_wn;

    std::vector<action_handler> action_handlers;

    void set_prop_internal(Atom prop, Atom type, int format, const unsigned char* data, unsigned long nitems);

    void get_prop_internal(Atom prop, Atom type, unsigned long offset, unsigned long length, unsigned char** data, unsigned long* nitems);

    point pos;
    point size;

public:
    client_window(Window& X11_window);
    client_window() = default;
    client_window(Display* disp);

    void resize(point p);
    void move(point p);
    void move_relative(point p);
    void resize_relative(point p);
    void min();
    // void max();
    void map();
    void unmap();
    void focus();
    void close();

    void add_action_handler(const automata& automata, const std::function<void(const event&)>& callback);
    void add_action_handler(const action_handler& handler);

    void pump_event(const XEvent& e);

    inline Window get_client_wn() const { return client_wn; }
    inline Window get_frame_wn() const { return frame_wn; }

    inline void set_frame_wn(Window w) { frame_wn = w; }

    inline bool operator==(const client_window& other) const { return client_wn == other.client_wn; }
    inline bool operator==(const Window& other) const { return client_wn == other; }

    inline bool operator!=(const client_window& other) const { return client_wn != other.client_wn; }
    inline bool operator!=(const Window& other) const { return client_wn != other; }

    template<typename T, typename API_T = typename atom<T>::type_information::type>
    inline void set_prop(const atom<T>& a, API_T value)
    {
        set_prop_internal(a.get(), atom<T>::type_information::type_atom, atom<T>::type_information::format, (unsigned char*)&value, atom<T>::type_information::nitems);
    }

    template<typename T, typename API_T = typename atom<T>::type_information::type>
    inline API_T get_prop(const atom<T>& a)
    {
        unsigned char* data;
        unsigned long nitems;
        get_prop_internal(a.get(), atom<T>::type_information::type_atom, 0, sizeof(T), &data, &nitems);
        return API_T(data);
    }

    void grab_button(unsigned int button, unsigned int modifiers, unsigned int event_mask);
    void grab_key(unsigned int keycode, unsigned int modifiers);
    void ungrab_button(unsigned int button, unsigned int modifiers);
    void ungrab_key(unsigned int keycode, unsigned int modifiers);
    void grab_pointer();
    void ungrab_pointer();

    void free();

    ~client_window();

    point get_pos() const { return pos; }
};