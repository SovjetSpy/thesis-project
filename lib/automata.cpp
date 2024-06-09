#include "automata.h"
#include "context.h"
#include <stdio.h>

const XEvent dummy_event = { .type = LASTEvent };


bool automata::run(client_window& w, const XEvent& e)
{
    // printf("running automata\n");
    s = t_fn(w, s, e);
    // printf("state: %d\n", s);
    // printf("end of run\n");
    return s == acc;
}

automata::automata(const tr_fn_t& transition_fn, state accepting_state)
{
    s = 0;
    t_fn = transition_fn;
    acc = accepting_state;

    t_fn(root_window, 0, dummy_event);
}

automata wildcard_any(unsigned int event)
{
    return automata([event](client_window& w, state s, const XEvent& e) -> state {
        if (e.type == event) return 1;
        return 0;
    }, (state)1);
}

bool is_dumy(const XEvent& e) {
    return &e == &dummy_event;
}

automata key_press_trigger(unsigned int key_code, unsigned int mod_mask) {
    return automata([=](client_window& w, state s, const XEvent& e) -> state {        
        return (e.type == KeyPress) && (e.xkey.keycode == key_code) && (e.xkey.state == mod_mask);
    }, (state)1);
}

automata button_press_trigger(unsigned int btn, unsigned int mod_mask) {
    return automata([=](client_window& w, state s, const XEvent& e) -> state { 
        // printf("btn: %d, mod: %d\n", btn, mod_mask);       
        return (e.type == ButtonPress) && (e.xbutton.button == btn) && (e.xbutton.state == mod_mask);
    }, (state)1);
}