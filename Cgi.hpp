#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>

class Cgi
{
    public:
        std::string cgiExtension;
        std::string cmd;
        std::string script;
        std::string arg;
        int         socket;

        Cgi();
        ~Cgi();

        void    selCgi();
        void    execPy();
        void    execSh();
        void    initCgi();
        char    **setArgs();
};

#endif

