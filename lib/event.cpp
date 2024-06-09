#include "event.h"


#define DYN_CASE(type) case type: { auto ev = Event_##type(e); fn(ev); } break


void dynamic_call(const XEvent& e, std::function<void(const event&)> fn)
{
    switch (e.type)
    {
        DYN_CASE(KeyPress);
        DYN_CASE(KeyRelease);
        DYN_CASE(ButtonPress);
        DYN_CASE(ButtonRelease);
        DYN_CASE(MotionNotify);
        DYN_CASE(EnterNotify);
        DYN_CASE(LeaveNotify);
        DYN_CASE(FocusIn);
        DYN_CASE(FocusOut);
        DYN_CASE(KeymapNotify);
        DYN_CASE(Expose);
        DYN_CASE(GraphicsExpose);
        DYN_CASE(NoExpose);
        DYN_CASE(VisibilityNotify);
        DYN_CASE(CreateNotify);
        DYN_CASE(DestroyNotify);
        DYN_CASE(UnmapNotify);
        DYN_CASE(MapNotify);
        DYN_CASE(MapRequest);
        DYN_CASE(ReparentNotify);
        DYN_CASE(ConfigureNotify);
        DYN_CASE(ConfigureRequest);
        DYN_CASE(GravityNotify);
        DYN_CASE(ResizeRequest);
        DYN_CASE(CirculateNotify);
        DYN_CASE(CirculateRequest);
        DYN_CASE(PropertyNotify);
        DYN_CASE(SelectionClear);
        DYN_CASE(SelectionRequest);
        DYN_CASE(SelectionNotify);
        DYN_CASE(ColormapNotify);
        DYN_CASE(ClientMessage);
        DYN_CASE(MappingNotify);
        DYN_CASE(GenericEvent);
    default:
        break;
    }
}