
#include "Application.h"

#include <stdexcept>
#include <iostream>

#include <chrono>
#include <thread>

int main()
{
    Application app;

    // 200 millisecond pause seems to resolve timing issue
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
 
    try
    {
        app.run();
    }
    catch (std::exception& e)
    {
        std::cout << "\n\nExceptionp: " << e.what() << std::endl;
    }

}