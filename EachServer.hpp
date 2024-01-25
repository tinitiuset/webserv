#pragma once


#include <iostream>
#include <vector>
#include <map>

class EachServer
{
    private:

        std::string                                                 _serverName;
        std::string                                                 _ip;
        std::string                                                 _root;
        std::map<int, std::string>                                  _errors;
        std::vector<int>                                            _ports;
        int                                                         _bodySize;
        std::map<std::string, std::map<std::string, std::string> >  _locations;
        
    public:
     
        void            setServerName(std::string servname);
        void            setIp(std::string ip);
        void            setRoot(std::string root);

        void            setErrors(int num, std::string msg);
        //set key error code as int and value error path

        void            setPorts(int port);
        void            setBodySize(int size);
        void            setLocationOutKey(std::string str);
        void            setLocationInValue(std::string outKey, std::string inKey, std::string inVal);

        /* getters */

        std::string         getServerName();
        std::string         getIp();
        std::string         getRoot();
        std::string         getErrorPath(int code);
        //returns the error page path of the error code passed
        
        std::vector<int>    getPorts();
        //returns a vector with the ports of the server

        int                 getBodySize();
        std::string         getLocationValue(std::string outKey, std::string inKey);
        //returns the value stored in each location. 
        //Recieves the outer key (Ex: "/") and the inner key (Ex: "file")


};