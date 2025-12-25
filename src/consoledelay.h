#pragma once

#include <thread>
#include <chrono>

class SLEEP
{
public:
    void delay(int ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
};

extern SLEEP sleep;