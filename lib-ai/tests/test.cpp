// test.cpp
#include <iostream>
#include "quickmaths.hpp"

int main()
{
    Initialize();
    auto result = GetResult("25 pounds in ounces");
    std::cout << std::endl
              << result.text << std::endl;
    std::cout << result.value << std::endl;
    return 0;
}