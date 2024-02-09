#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <map>

class Cgi
{
    private:

        int                                 _servIdx;
        std::string                         _method;
        std::string                         _uri;
        std::string                         _quStr;
        char                                **_env;
        int                                 _fd[2];
        int                                 _fdOut;
    
    public:

        Cgi(int servI, int  port, std::string reqLine, std::map<std::string, std::string> headers, std::string _body);
        ~Cgi();

        void            selMethod();
        void            setGETEnv();
        void            setPost();
        void            setStdOut();


        void            execPy();
        void            execSh();
        std::string     initCgi();
};

std::string getPath(const std::string &line);
std::string getQuStr(const std::string &line);

#endif

