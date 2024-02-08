#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <map>

class Cgi
{
    private:

        std::string                         _reqLine;
        std::map<std::string, std::string>  _headers;
	    std::string                         _body;
        char                                **_env;
        int                                 _servIdx;
    
    public:

        Cgi(std::string reqLine, std::map<std::string, std::string> headers, std::string _body);
        ~Cgi();

        void            selMethod();
        void            setGetEnv();
        void            setPostEnv();
        void            setStdOut();


        void            execPy();
        void            execSh();
        std::string     initCgi();
};

#endif

