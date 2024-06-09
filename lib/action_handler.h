#pragma once
#include "automata.h"
#include "event.h"

using callback_t = const std::function<void(const event&)>&;


class client_window;

using lambda_fn_type = decltype([](const event& e) mutable {});

class action_handler
{
    automata state_machine;
    std::function<void(const event&)> callback;
    std::function<std::function<void(const event&)>()> get_callback;
    bool reset_callback;
public:
    action_handler(const automata& automata, const std::function<void(const event&)>& callback);
    action_handler(const action_handler&) = default;

    void pump_event(client_window& w, const XEvent& e);
};