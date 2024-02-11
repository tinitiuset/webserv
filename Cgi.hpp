#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <map>

class Cgi
{
    private:

        int				_servIdx;
        std::string     _method;
        std::string     _uri;
        std::string     _quStr;
        std::string     _interpret;
        char            **_env;
        int				_fd_parent_to_child[2];
    	int 			_fd_child_to_parent[2];
		std::string		_resp;
    
    public:

        Cgi(std::string method, std::string uri, std::string qustr);
        ~Cgi();

        void            selMethod();
        void            set4GETEnv();
        void            set4Post();
        void            setStdOut();


        void            execPy();
        void            execSh();
        std::string     initCgi();
};

std::string getPath(const std::string &line);
std::string getQuStr(const std::string &line);

#endif

