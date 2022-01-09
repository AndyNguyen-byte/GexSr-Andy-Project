#include "Application.h"

#include <stdexcept>
#include <iostream>

int main()
{
    Application app;

    try
    {
        app.run();
    }
    catch (std::exception& e)
    {
        std::cout << "\n\nException " << e.what() << std::endl;
    }

}