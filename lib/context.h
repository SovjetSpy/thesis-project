#pragma once
#include <iostream>
#include "util.h"
#include <vector>
#include <unordered_map>
#include "window.h"
#include <memory>

class ctx {

    

    Display* disp;
    std::shared_ptr<client_window> real_root_wn;
    Window shadow_root_wn;
    std::unordered_map<Window, std::shared_ptr<client_window>> managed_windows;

    void setup_shadow_root();

    void proces_event(XEvent& e);

    void create_window(XCreateWindowEvent& e);
    void destroy_window(XDestroyWindowEvent& e);

    point cursor_pos;
    point cursor_pos_delta;

public:

    ctx();

    static ctx& get();

    inline Display* get_disp() const { return disp; }

    inline client_window& get_root_window() { return *real_root_wn.get(); }

    inline Window get_shadow_root() { return shadow_root_wn; }

    Atom make_atom(const char* name, bool if_exists);

    bool setup();

    void run();

    inline std::shared_ptr<client_window> get_window(Window w) { return managed_windows[w]; };

    inline void add_action_handler(const automata& automata, const std::function<void(const event&)>& callback)
    {
        real_root_wn->add_action_handler(automata, callback);
    }

    inline void add_action_handler(const action_handler& handler)
    {
        real_root_wn->add_action_handler(handler);
    }

    inline point get_cursor_pos() { return cursor_pos; }
    inline point get_cursor_delta() { return cursor_pos_delta; }

};

#define context (ctx::get())
#define root_window (ctx::get().get_root_window())