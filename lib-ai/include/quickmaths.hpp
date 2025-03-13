#pragma once
#include <iostream>

extern "C"
{
    double GetResult(std::string);
    double Calculate(std::string);
    void Shutdown();
    void Initialize();
}