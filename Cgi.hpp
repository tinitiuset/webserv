#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <map>

class Cgi
{
    private:

        int                                 _servIdx;
        int                                 _port;
        std::string                         _reqLine;
        std::map<std::string, std::string>  _headers;
	    std::string                         _body;
        char                                **_env;
        int                                 _fd[2];
        int                                 _fdOut;
        std::string                         _method;
    
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

#endif

