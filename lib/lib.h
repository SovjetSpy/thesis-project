#pragma once
#include "context.h"
#include "atom.h"
#include "window.h"
#include "action_handler.h"
#include <memory>

void setup_wm();


int main(int argc, char** argv)
{
    try
    {
        if (!context.setup())
        {
            std::cerr << "Failed to setup X11. Other wm found." << std::endl;
            return 1;
        }
        setup_wm();
    
        context.run();
    
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
}


// atom<int> hallo("hallo");

