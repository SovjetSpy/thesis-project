g++ main.cpp lib/*.cpp -lX11 -std=gnu++2b



XEPHYR=$(whereis -b Xephyr | cut -f2 -d' ')

xinit ./xinitrc -- \
    "$XEPHYR" \
        :100 \
        -ac \
        -screen 800x600 \
        -host-cursor