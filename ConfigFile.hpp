#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "EachServer.hpp"


class ConfigFile
{
    private:

    public:
        EachServer  *_serverArr;
        int         _servAmount;
                        
        ConfigFile(char *str, int n);
        ~ConfigFile();

};

#endif
