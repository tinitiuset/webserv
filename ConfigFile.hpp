#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <EachServer.hpp>

class ConfigFile
{
    private:
        
        std::vector<EachServer *>  _serverVect;


    public:
        ConfigFile(char *str);
        ~ConfigFile();

        




};
