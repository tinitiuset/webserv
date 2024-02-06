#include "../Cgi.hpp"


int main(int ac, char **av)
{
    Cgi app;

    app.initCgi();
    std::cout << "En main despues del todo tinglao" << std::endl;
}
