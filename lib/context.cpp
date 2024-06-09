#include "context.h"
#include <memory>


int OnXError(Display *display, XErrorEvent *e)
{
    const int MAX_ERROR_TEXT_LENGTH = 1024;
    char error_text[MAX_ERROR_TEXT_LENGTH];
    XGetErrorText(display, e->error_code, error_text, sizeof(error_text));
    std::cerr << "Received X error:\n"
              << "    Request: " << int(e->request_code)
              << " - " << XRequestCodeToString(e->request_code) << "\n"
              << "    Error code: " << int(e->error_code)
              << " - " << error_text << "\n"
              << "    Resource ID: " << e->resourceid << "\n";
    // The return value is ignored.
    return 0;
}

bool wm_detected_ = false;

int OnWMDetected(Display *display, XErrorEvent *e)
{
    if (static_cast<int>(e->error_code) == BadAccess)
    {
        wm_detected_ = true;
    }

    return 0;
}

ctx::ctx() {
    disp = XOpenDisplay(NULL);
}

ctx& ctx::get() {
    static auto ptr = ctx();

    return ptr;
}

Atom ctx::make_atom(const char* name, bool if_exists)
{
    return XInternAtom(disp, name, if_exists);
}

bool ctx::setup()
{
    real_root_wn = std::make_shared<client_window>(disp);
    // managed_windows[real_root_wn->get_client_wn()] = real_root_wn;

    setup_shadow_root();
    XSetErrorHandler(OnWMDetected);
    XSelectInput(disp, real_root_wn->get_client_wn(), SubstructureRedirectMask | SubstructureNotifyMask);
    XSync(disp, false);

    if (wm_detected_)
        return false;

    XSetErrorHandler(OnXError);

    return true;
}

void ctx::run()
{
    XGrabServer(disp);

    Window root, parrent;

    Window* children;
    unsigned int num;

    XQueryTree(disp, real_root_wn->get_client_wn(), &root, &parrent, &children, &num);

    for (int i = 0; i < num; i++)
    {
        Window w = children[i];
        XWindowAttributes attrs;
        XGetWindowAttributes(disp, w, &attrs);

        XEvent e;
        e.type = CreateNotify;
        e.xcreatewindow = {
            .type = CreateNotify,
            .serial = 0,
            .send_event = false,
            .display = disp,
            .parent = parrent,
            .window = w,
            .x = attrs.x,
            .y = attrs.y,
            .width = attrs.width,
            .height = attrs.height,
            .border_width = attrs.border_width,
            .override_redirect = false
        };

        proces_event(e);
    }

    XUngrabServer(disp);
    XFree(children);

    unsigned int mask;
    point dumy;

    XQueryPointer(disp, real_root_wn->get_client_wn(), &root, &parrent, &cursor_pos.x, &cursor_pos.y, &dumy.x, &dumy.y, &mask);

    cursor_pos_delta = {0, 0};

    XEvent e;
    while (true)
    {
        XNextEvent(this->disp, &e);
        if (e.type == MotionNotify)
        {
            while (XCheckTypedWindowEvent(disp, e.xmotion.window, MotionNotify, &e)) {}
            cursor_pos_delta = {e.xmotion.x_root - cursor_pos.x, e.xmotion.y_root - cursor_pos.y};
            cursor_pos = {e.xmotion.x_root, e.xmotion.y_root};
        }
            
        proces_event(e);
    }
}

void ctx::proces_event(XEvent& e)
{
    switch (e.type)
    {
        case CreateNotify:
            if (e.xcreatewindow.window == shadow_root_wn) return;
            create_window(e.xcreatewindow);
        case CirculateNotify:	
        case ConfigureNotify:	
        case DestroyNotify:	
        case GravityNotify:	
        case MapNotify:	
        case ReparentNotify:	
        case UnmapNotify:	
        case CirculateRequest:	
        case ConfigureRequest:	
        case MapRequest:
            real_root_wn->pump_event(e);
            if (e.type == DestroyNotify)
                destroy_window(e.xdestroywindow);
            return;
        
        default:
            break;
    }

    if (managed_windows.find(e.xany.window) != managed_windows.end())
    {
        managed_windows[e.xany.window]->pump_event(e);
    }
}

void ctx::create_window(XCreateWindowEvent& e)
{
    managed_windows[e.window] = std::shared_ptr<client_window>(nullptr);
    managed_windows[e.window] = std::make_shared<client_window>(e.window);
}

void ctx::destroy_window(XDestroyWindowEvent& e)
{
    managed_windows.erase(e.window);
}

void ctx::setup_shadow_root()
{
    const unsigned long BORDER_COLOR = 0x000000;
    const unsigned long BG_COLOR = 0x0000ff;

    XWindowAttributes  attr;
    XGetWindowAttributes(disp, DefaultRootWindow(disp), &attr);

    shadow_root_wn = XCreateSimpleWindow(disp, DefaultRootWindow(disp), 0, 0, attr.width, attr.height, 0, BORDER_COLOR, BG_COLOR);
    XMapWindow(disp, shadow_root_wn);
}

