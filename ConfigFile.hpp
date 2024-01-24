#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "EachServer.hpp"

class ConfigFile
{
    private:
        
        //std::vector<EachServer *>  _serverVect;
        EachServer  *_serverArr;
        int         _servAmount;                   


    public:
        ConfigFile(std::ifstream &file, int n);
        ~ConfigFile();

        




};
