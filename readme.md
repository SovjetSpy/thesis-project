# Example DSL for the creation of X11 window managers

This file contains the projectfiles that are part of my thesis. The `lib` folder contains the source code of the implemented eDSL. The `main.cpp` contains the example window manager that is created with the eDSL.

## how to run
Install the dependencies with
```bash
sudo apt-get install \
    build-essential libx11-dev \
    xserver-xephyr xinit x11-apps xterm
```

Then run the following command to build and start the window manager
```bash
./test.sh
```

A window will open a window with a embedded Xserver with the example window manager and test aplications.

## how to use the example window manager
The window manager has the following keybindings:
- click on a window to focus
- `ALT + mouse1` to move a window
- `CTRL + mouse1` to resize a window