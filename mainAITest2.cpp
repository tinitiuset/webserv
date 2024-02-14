#include "Autoindex.hpp"

int main()
{
    Autoindex inst("127.0.0.1", "/ai", 8080, "/www", "/ai/kosikas");

    inst.buildAI();
    return (0);
}
