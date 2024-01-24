#pragma once


#include <iostream>
#include <vector>
#include <map>

class EachServer
{
    private:

        std::string                                                 _server_name;
        std::string                                                 _ip;
        std::string                                                 _root;
        std::map<int, std::string>                                  _errors;
        std::vector<int>                                            _ports;
        std::map<std::string, std::map<std::string, std::string> >   _locations;
        
    public:
     
        void    setServerName(std::string servname);
        void    setIp(std::string ip);
        void    setRoot(std::string root);
        void    setErrors(int num, std::string msg);
        void    setPorts(int port);

};