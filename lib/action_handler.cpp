#include "action_handler.h"
#include "window.h"
#include <stdio.h>

action_handler::action_handler(const automata& automata, const std::function<void(const event&)>& pcallback) :
    state_machine(automata), callback{pcallback}, reset_callback(true) {
        get_callback = [pcallback]() { return pcallback; };
    }

void action_handler::pump_event(client_window& w, const XEvent& e)
{
    // printf("pump_event\n");
    if (state_machine.run(w, e))
    {
        dynamic_call(e, callback);
    } else if (state_machine.is_reset())
    {
        // printf("reset, %d\n", state_machine.get_state());
        reset_callback = true;
    }
    // printf("end of pump_event\n");
}