#include "window.h"
#include "context.h"
#include <algorithm>

client_window::client_window(Display* disp) : client_wn(DefaultRootWindow(disp)), frame_wn(client_wn) {}

client_window::client_window(Window& X11_window) : client_wn(X11_window), frame_wn(X11_window)
{
    // XSelectInput(context.disp, frame_wn, StructureNotifyMask);
    // XMapWindow(context.get_disp(), frame_wn);

    const unsigned int BORDER_WIDTH = 3;
    const unsigned long BORDER_COLOR = 0xff0000;
    const unsigned long BG_COLOR = 0x00ff00;

    auto disp = context.get_disp();

    XWindowAttributes x_window_attrs;
    XGetWindowAttributes(disp, this->client_wn, &x_window_attrs);

    printf("x: %d, y: %d, width: %d, height: %d\n", x_window_attrs.x, x_window_attrs.y, x_window_attrs.width, x_window_attrs.height);

    // resize({x_window_attrs.width, x_window_attrs.height});
    // move({x_window_attrs.x, x_window_attrs.y});

     frame_wn = XCreateSimpleWindow(
       disp,
       context.get_shadow_root(),
       x_window_attrs.x,
       x_window_attrs.y,
       x_window_attrs.width,
       x_window_attrs.height,
       BORDER_WIDTH,
       BORDER_COLOR,
       BG_COLOR);

     XReparentWindow(
       disp,
       client_wn,
       frame_wn,
       0, 0);

    XSelectInput(disp, frame_wn, SubstructureRedirectMask | SubstructureNotifyMask);

    pos = {x_window_attrs.x, x_window_attrs.y};
    size = {x_window_attrs.width, x_window_attrs.height};
}


void client_window::resize(point p)
{
    size = {std::max(p.x, 50), std::max(p.y, 50)};
    XResizeWindow(context.get_disp(), frame_wn, p.x, p.y);
    XResizeWindow(context.get_disp(), client_wn, p.x, p.y);
}

void client_window::move(point p)
{
    pos = p;
    XMoveWindow(context.get_disp(), frame_wn, p.x, p.y);
}

void client_window::move_relative(point p)
{
    point new_pos = {pos.x + p.x, pos.y + p.y};
    move(new_pos);
}

void client_window::resize_relative(point p)
{
    point new_pos = {size.x + p.x, size.y + p.y};
    resize(new_pos);
}

void client_window::min()
{
    XIconifyWindow(context.get_disp(), client_wn, 0);
}

// void client_window::max()
// {
//     // XEvent e;
//     // e.type = ClientMessage;
//     // e.xclient = {
//     //     .type = ClientMessage,
//     //     .serial = 0,
//     //     .send_event = false,
//     //     .display = context.get_disp(),
//     //     .window = client_wn,
//     //     .message_type = context.make_atom("_NET_WM_STATE", true),
//     //     .format = 32,
//     //     .data = {
//     //         .l = {
//     //             1,
//     //             context.make_atom("_NET_WM_STATE_MAXIMIZED_VERT", true),
//     //             context.make_atom("_NET_WM_STATE_MAXIMIZED_HORZ", true),
//     //             0,
//     //             0
//     //         }
//     //     }
//     // };
// 
//     // XSendEvent(context.get_disp(), DefaultRootWindow(context.get_disp()), false, SubstructureRedirectMask | SubstructureNotifyMask, &e);
// }

void client_window::map()
{
    XMapWindow(context.get_disp(), frame_wn);
    XMapWindow(context.get_disp(), client_wn);
}

void client_window::unmap()
{
    XUnmapWindow(context.get_disp(), frame_wn);
    XUnmapWindow(context.get_disp(), client_wn);
}

void client_window::focus() 
{
    XSetInputFocus(context.get_disp(), frame_wn, RevertToParent, CurrentTime);
    XRaiseWindow(context.get_disp(), frame_wn);
}

void client_window::add_action_handler(const automata& automata, const std::function<void(const event&)>& callback)
{
    action_handlers.emplace_back(automata, callback);
}

void client_window::add_action_handler(const action_handler& handler)
{
    action_handlers.push_back(handler);
}

void client_window::pump_event(const XEvent& e)
{
    // printf("begin pumping event\n");
    for (auto& handler : action_handlers)
    {
        // printf("pumping event\n");
        handler.pump_event(*this, e);
    }
    // printf("end of pump\n");
}

void client_window::set_prop_internal(Atom prop, Atom type, int format, const unsigned char* data, unsigned long nitems)
{
    XChangeProperty(context.get_disp(), client_wn, prop, type, format, PropModeReplace, data, nitems);
}

void client_window::get_prop_internal(Atom prop, Atom type, unsigned long offset, unsigned long length, unsigned char** data, unsigned long* nitems)
{
    Atom actual_type;
    int actual_format;
    unsigned long bytes_after;
    XGetWindowProperty(context.get_disp(), client_wn, prop, offset, length, false, type, &actual_type, &actual_format, nitems, &bytes_after, data);
}

void client_window::grab_button(unsigned int button, unsigned int modifiers, unsigned int event_mask)
{
    XGrabButton(context.get_disp(), button, modifiers, client_wn, false, event_mask, GrabModeAsync, GrabModeAsync, None, None);
}

void client_window::grab_key(unsigned int keycode, unsigned int modifiers)
{
    XGrabKey(context.get_disp(), keycode, modifiers, frame_wn, true, GrabModeAsync, GrabModeAsync);
}

void client_window::ungrab_button(unsigned int button, unsigned int modifiers)
{
    // XUngrabButton(context.get_disp(), button, modifiers, frame_wn);
}

void client_window::ungrab_key(unsigned int keycode, unsigned int modifiers)
{
    XUngrabKey(context.get_disp(), keycode, modifiers, frame_wn);
}

void client_window::grab_pointer()
{
    // XGrabPointer(context.get_disp(), frame_wn, true, event_mask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
}

void client_window::ungrab_pointer()
{
    // XUngrabPointer(context.get_disp(), CurrentTime);
}

void client_window::free()
{
    XDestroyWindow(context.get_disp(), frame_wn);
    // XDestroyWindow(context.get_disp(), client_wn);
}

client_window::~client_window()
{
    free();
}

void client_window::close()
{
    XKillClient(context.get_disp(), client_wn);
}