#include "ConfigFile.hpp"
#include "EachServer.hpp"

ConfigFile::ConfigFile(std::ifstream &file, int n)
{
    _serverArr = new EachServer[n];

    std::string     line;
    int             i = -1;
    
    while (std::getline(file, line))
    {
        
        if (line.find("server:") != std::string::npos)
            i++;
        else if (line.find("error ") != std::string::npos)
        {
            _serverArr[i].setErrors(std::atoi(line.substr(10, 3).c_str()), line.substr(15));
            //std::cout << ">>>>>[" <<line.substr(10, 3) << "]" << std::endl;
            //std::cout << "*****" << "[" <<line.substr(15) << "]" << std::endl;
        }
        else if (line.find("server_name: ") != std::string::npos)
        {
            _serverArr[i].setServerName = 
            std::cout << "[" <<line.substr(line.find("server_name:") + std::string("server_name :").length()) << "]" << std::endl;
        }    
        else if (line.find("ip: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("ip: ") + std::string("ip: ").length()) << "]" << std::endl;
        else if (line.find("root: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("root: ") + std::string("root: ").length()) << "]" << std::endl;
        else if (line.find("port: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("port: ") + std::string("port: ").length()) << "]" << std::endl;
        else if (line.find("body_size: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("body_size: ") + std::string("body_size: ").length()) << "]" << std::endl;
        else if (line.find("location: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("location: ") + std::string("location: ").length()) << "]" << std::endl;
        else if (line.find("file: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("file: ") + std::string("file: ").length()) << "]" << std::endl;
        else if (line.find("redirect: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("redirect: ") + std::string("redirect: ").length()) << "]" << std::endl;
        else if (line.find("autoindex: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("autoindex: ") + std::string("autoindex: ").length()) << "]" << std::endl;
        else if (line.find("GET: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("GET: ") + std::string("GET: ").length()) << "]" << std::endl;
        else if (line.find("POST: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("POST: ") + std::string("POST: ").length()) << "]" << std::endl;
        else if (line.find("DELETE: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("DELETE: ") + std::string("DELETE: ").length()) << "]" << std::endl;
        else if (line.find("upload_folder: ") != std::string::npos)
            std::cout << "[" <<line.substr(line.find("upload_folder: ") + std::string("upload_folder: ").length()) << "]" << std::endl;
        
    }

 

    return ;


}