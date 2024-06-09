#pragma once
#include <functional>
#include "util.h"

class event_base {
    XAnyEvent any;
public:
    event_base(const XAnyEvent& any) : any(any) {};
    event_base() = default;
    virtual ~event_base() = default;

    virtual inline Window get_window() const { return any.window; }
    virtual inline unsigned int get_type() const { return any.type; }
};


using event = event_base&;


#define EVENT_TYPE_DEF(name, entry) \
using Event_##name##_entry_t = typeof(((XEvent*)0)->entry); \
class Event_##name : public event_base, public Event_##name##_entry_t { \
public: \
    Event_##name(const XEvent& ev) : event_base(ev.xany), Event_##name##_entry_t(ev.entry) {} \
    Event_##name() = default; \
    virtual ~Event_##name() = default; \
}

EVENT_TYPE_DEF(KeyPress, xkey);
EVENT_TYPE_DEF(KeyRelease, xkey);
EVENT_TYPE_DEF(ButtonPress, xbutton);
EVENT_TYPE_DEF(ButtonRelease, xbutton);
EVENT_TYPE_DEF(MotionNotify, xmotion);
EVENT_TYPE_DEF(EnterNotify, xcrossing);
EVENT_TYPE_DEF(LeaveNotify, xcrossing);
EVENT_TYPE_DEF(FocusIn, xfocus);
EVENT_TYPE_DEF(FocusOut, xfocus);
EVENT_TYPE_DEF(KeymapNotify, xkeymap);
EVENT_TYPE_DEF(Expose, xexpose);
EVENT_TYPE_DEF(GraphicsExpose, xgraphicsexpose);
EVENT_TYPE_DEF(NoExpose, xnoexpose);
EVENT_TYPE_DEF(VisibilityNotify, xvisibility);
EVENT_TYPE_DEF(CreateNotify, xcreatewindow);
EVENT_TYPE_DEF(DestroyNotify, xdestroywindow);
EVENT_TYPE_DEF(UnmapNotify, xunmap);
EVENT_TYPE_DEF(MapNotify, xmap);
EVENT_TYPE_DEF(MapRequest, xmaprequest);
EVENT_TYPE_DEF(ReparentNotify, xreparent);
EVENT_TYPE_DEF(ConfigureNotify, xconfigure);
EVENT_TYPE_DEF(ConfigureRequest, xconfigurerequest);
EVENT_TYPE_DEF(GravityNotify, xgravity);
EVENT_TYPE_DEF(ResizeRequest, xresizerequest);
EVENT_TYPE_DEF(CirculateNotify, xcirculate);
EVENT_TYPE_DEF(CirculateRequest, xcirculaterequest);
EVENT_TYPE_DEF(PropertyNotify, xproperty);
EVENT_TYPE_DEF(SelectionClear, xselectionclear);
EVENT_TYPE_DEF(SelectionRequest, xselectionrequest);
EVENT_TYPE_DEF(SelectionNotify, xselection);
EVENT_TYPE_DEF(ColormapNotify, xcolormap);
EVENT_TYPE_DEF(ClientMessage, xclient);
EVENT_TYPE_DEF(MappingNotify, xmapping);
EVENT_TYPE_DEF(GenericEvent, xgeneric);

#undef EVENT_TYPE_DEF

void dynamic_call(const XEvent& e, std::function<void(const event&)> fn);