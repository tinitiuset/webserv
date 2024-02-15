#include "Autoindex.hpp"

#include <iostream>
#include <vector>
#include <string>

 
int main()
{
    Autoindex inst(0, 0, "127.0.0.1", "/ai", 8080, "/www/", "/ai/");

    //std::cout << inst.isAI() << std::endl;
    inst.buildAI();
    return (0);
}

