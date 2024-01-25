#include "ConfigFile.hpp"
#include "EachServer.hpp"

class EachServer;

ConfigFile::ConfigFile(char *str, int n): _servAmount(n)
{
    _serverArr = new EachServer[n];
    std::ifstream file;
    file.open(str);
    std::string     line;
    std::string     keyOuter;
    int             i = -1;
    
    while (std::getline(file, line))
    {
        if (line.find("server:") != std::string::npos)
            i++;
        else if (line.find("server_name: ") != std::string::npos)
            _serverArr[i].setServerName(line.substr(15));
        else if (line.find("error ") != std::string::npos)
            _serverArr[i].setErrors(std::atoi(line.substr(10, 3).c_str()), line.substr(15));
        else if (line.find("ip: ") != std::string::npos)
            _serverArr[i].setIp(line.substr(6));
        else if (line.find("root: ") != std::string::npos)
            _serverArr[i].setRoot(line.substr(8));
        else if (line.find("port: ") != std::string::npos)
            _serverArr[i].setPorts(std::atoi(line.substr(8).c_str()));
        else if (line.find("body_size: ") != std::string::npos)
            _serverArr[i].setBodySize(std::atoi(line.substr(13).c_str()));
        else if (line.find("location: ") != std::string::npos)
        {
            keyOuter = line.substr(12);
            _serverArr[i].setLocationOutKey(keyOuter);
        }
        else if (line.find("file: ") != std::string::npos)
            _serverArr[i].setLocationInValue(keyOuter, "file", line.substr(10));
        else if (line.find("redirect: ") != std::string::npos)
            _serverArr[i].setLocationInValue(keyOuter, "redirect", line.substr(14));
        else if (line.find("autoindex: ") != std::string::npos)
            _serverArr[i].setLocationInValue(keyOuter, "autoindex", line.substr(15));
        else if (line.find("GET: ") != std::string::npos)
            _serverArr[i].setLocationInValue(keyOuter, "GET", line.substr(11));
        else if (line.find("POST: ") != std::string::npos)
            _serverArr[i].setLocationInValue(keyOuter, "POST", line.substr(12));
        else if (line.find("DELETE: ") != std::string::npos)
            _serverArr[i].setLocationInValue(keyOuter, "DELETE", line.substr(14));
        else if (line.find("upload_folder: ") != std::string::npos)
            _serverArr[i].setLocationInValue(keyOuter, "upload_folder", line.substr(19));
        
    }
    file.close();
    return ;
}

ConfigFile::~ConfigFile()
{
    delete[] _serverArr;
    return ;
}