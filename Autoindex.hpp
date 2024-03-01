

#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include "defaults.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include "Utils.hpp"

class Autoindex
{
    private:
        int         _servIdx;
        int         _locationIdx;
        std::string _serverName;
        std::string _serverPath;
        std::string _uri;
        std::string _locName;

        std::string _pathReplaced;
        std::string _host;


    public:
        Autoindex(int servIdx, int locationIdx, std::string serverName, std::string locName, int port, std::string root, std::string uri);

        bool            isAI();
        std::string     buildAI();
        std::string     getPreviousUri(std::string uri);

};


#endif

