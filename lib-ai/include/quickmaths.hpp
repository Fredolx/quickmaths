#pragma once
#include <iostream>

extern "C"
{
    struct StringDouble
    {
        const char *text;
        double value;
    };
    StringDouble GetResult(const char *query);
    double Calculate(std::string);
    void Shutdown();
    void Initialize();
}