#pragma once
#include <functional>
#include "util.h"



class client_window;

using state = unsigned int;
using tr_fn_t = std::function<state(client_window&, state, const XEvent&)>;

// using transition_fnt = const tr_fn_t&;

class automata
{
    state s = 0;
    tr_fn_t t_fn;
    state acc;

public:
    automata(const tr_fn_t& transition_fn, state accepting_state);

    bool run(client_window& w, const XEvent& e);

    inline bool is_reset() { return s == 0; }

    inline state get_state() { return s; }
};


automata wildcard_any(unsigned int event);

bool is_dumy(const XEvent& e);

automata key_press_trigger(unsigned int key_code, unsigned int mod_mask);
automata button_press_trigger(unsigned int key_code, unsigned int mod_mask);