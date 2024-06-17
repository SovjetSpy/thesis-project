#include "lib/lib.h"

auto a = automata([](client_window& w, state s, const XEvent& e) -> state {
        if (e.type == KeyPress) {
            // printf("keycode: %d, sym: %u, st %u\n", e.xkey.keycode, XKeysymToKeycode(context.get_disp(), XK_Tab), e.xkey.state);
            if (e.xkey.keycode == XKeysymToKeycode(context.get_disp(), XK_Tab) && e.xkey.state == Mod1Mask) {
                return 1;
            }
        }

        if (e.type == KeyRelease) {
            // printf("keycode: %d, sym: %u, st %u\n", e.xkey.keycode, XKeysymToKeycode(context.get_disp(), XK_Tab), e.xkey.state);
            if (e.xkey.keycode == XKeysymToKeycode(context.get_disp(), XK_Tab) && e.xkey.state == Mod1Mask) {
                return 0;
            }
        }

        return 0;
    }, (state)1);

auto window_move_automata = automata([](client_window& w, state s, const XEvent& e) -> state {
        switch (s)
        {
            case 0:
                if (e.type == ButtonPress && e.xbutton.button == Button1 && e.xbutton.state == Mod1Mask) {
                    return 1;
                } else
                    return 0;
            case 1:
                if (e.type == MotionNotify) {
                    return 2;
                } else if (e.type == ButtonRelease && e.xbutton.button == Button1) {
                    return 0;
                } else
                    return 1;
            case 2:
                if (e.type == MotionNotify) {
                    return 2;
                } else if (e.type == ButtonRelease && e.xbutton.button == Button1) {
                    return 0;
                } else
                    return 1;
        }
    }, (state)2);

auto resize_automata = automata([](client_window& w, state s, const XEvent& e) -> state {

        switch (s)
        {
            case 0:
                if (e.type == ButtonPress && e.xbutton.button == Button1 && e.xbutton.state == ControlMask) {
                    return 1;
                } else
                    return 0;
            case 1:
                if (e.type == MotionNotify) {
                    return 2;
                } else if (e.type == ButtonRelease && e.xbutton.button == Button1) {
                    return 0;
                } else
                    return 1;
            case 2:
                if (e.type == MotionNotify) {
                    return 2;
                } else if (e.type == ButtonRelease && e.xbutton.button == Button1) {
                    return 0;
                } else
                    return 1;
        }
    }, (state)2);


atom<Window_wrapper> hallo("root_window");
atom<std::string> wnd_info("NAME");


void setup_wm()
{

    context.add_action_handler(wildcard_any(CreateNotify), [&](const event& e){
        auto& ev = (Event_CreateNotify&)e;

        printf("CreateNotify\n");
        printf("window: %lu\n", ev.window);
        printf("parent: %lu\n", ev.parent);
        printf("x: %d\n", ev.x);
        printf("y: %d\n", ev.y);
        printf("width: %d\n", ev.width);
        printf("height: %d\n", ev.height);
        printf("border_width: %d\n", ev.border_width);


        auto wn = context.get_window(ev.window);
        wn->map();

        wn->set_prop(hallo, root_window.get_client_wn());
        // wn->set_prop(wnd_info, std::string("hallo"));

        wn->add_action_handler(wildcard_any(MapRequest), [&](const event& e){
            auto& ev = (Event_MapRequest&)e;

            std::cout << "maprequest\n";
            
            auto wn = context.get_window(ev.window);
            wn->map();

        });

        std::shared_ptr<point> drag_start = std::make_shared<point>();

        wn->grab_key(XKeysymToKeycode(context.get_disp(), XK_Tab), Mod1Mask);
        wn->grab_button(Button1, Mod1Mask, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
        wn->grab_button(Button1, ControlMask, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
        wn->grab_button(Button1, 0, ButtonPressMask);
        wn->add_action_handler(a, [](const event &e) {
            auto& ev = (Event_DestroyNotify&)e;
            std::cout << "alt+tab\n"; 

            // auto wn = context.get_window(ev.window);
            // std::cout << wn->get_prop(wnd_info) << std::endl;

        });

        auto drag_resize_start = [wn, drag_start](const event &e) {
            auto& ev = (Event_ButtonPress&)e;
            printf("drag resize start\n");
            wn->focus();

            drag_start->x = ev.x_root;
            drag_start->y = ev.y_root;
        };

        wn->add_action_handler(button_press_trigger(Button1, Mod1Mask), drag_resize_start);

        wn->add_action_handler(button_press_trigger(Button1, ControlMask), drag_resize_start);

        wn->add_action_handler(button_press_trigger(Button1, 0), drag_resize_start);

        wn->add_action_handler(window_move_automata, [wn, drag_start](const event &e) mutable {
            auto& ev = (Event_MotionNotify&)e;

            point delta = {ev.x_root - drag_start->x, ev.y_root - drag_start->y};
            drag_start->x = ev.x_root;
            drag_start->y = ev.y_root;

            wn->move_relative(delta);
        });

        wn->add_action_handler(resize_automata, [wn, drag_start](const event &e) mutable {
            auto& ev = (Event_MotionNotify&)e;

            point delta = {ev.x_root - drag_start->x, ev.y_root - drag_start->y};
            drag_start->x = ev.x_root;
            drag_start->y = ev.y_root;

            wn->resize_relative(delta);
        });

        wn->add_action_handler(wildcard_any(MapRequest), [wn](const event& e){
            auto& ev = (Event_DestroyNotify&)e;

            wn->map();
        });

        std::cout << "CreateNotify\n";
    });


    context.add_action_handler(wildcard_any(ConfigureRequest), [](const event& e){
        auto& ev = (Event_ConfigureRequest&)e;

        auto wn = context.get_window(ev.window);

        if (ev.value_mask & CWX|CWY)
            wn->move({ev.x, ev.y});

        if (ev.value_mask & CWWidth|CWHeight)
            wn->resize({ev.width, ev.height});
    });
    

    context.add_action_handler(wildcard_any(DestroyNotify), [](const event& e){
        auto& ev = (Event_DestroyNotify&)e;

        context.get_window(ev.window)->unmap();
        context.get_window(ev.window)->free();
    });

    
}
