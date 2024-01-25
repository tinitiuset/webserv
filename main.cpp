#include <iostream>
#include <fstream>
#include <sstream>
#include <fstream>
#include <vector>
#include "ConfigFile.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
        return (std::cout << "Unexpected argument number" << std::endl, 0);

    std::ifstream   file(av[1]);
    if (!file.is_open())
		return (std::cerr << "Unexpected config file" << std::endl, 0);

    std::string     line;
    int             count = 0;
    while (std::getline(file, line)) 
        if (line.find("server:") != std::string::npos)
            ++count;
    if (count < 1)
        return (std::cerr << "Invalid config file" << std::endl, 0);
    file.close();
    //std::cout << "count " << count << std::endl;
    ConfigFile confFile(av[1], count);

    std::cout << confFile._serverArr[0].getServerName() << std::endl;
    
    std::cout << confFile._serverArr[0].getIp() << std::endl;
    
    std::vector<int>::iterator it;
    for (it = confFile._serverArr[0].getPorts().begin(); it != confFile._serverArr[0].getPorts().end(); ++it)
        std::cout << *it << std::endl;
    
    std::cout << confFile._serverArr[0].getRoot() << std::endl;
    
    std::cout << confFile._serverArr[0].getBodySize() << std::endl;
    
    std::cout << confFile._serverArr[0].getErrorPath(404) << std::endl;
    std::cout << confFile._serverArr[0].getErrorPath(405) << std::endl;
    std::cout << confFile._serverArr[0].getErrorPath(406) << std::endl;
    
    std::cout << confFile._serverArr[0].getLocationValue("/", "file") << std::endl;
    std::cout << confFile._serverArr[2].getLocationValue("/images/sample", "POST") << std::endl;
    std::cout << confFile._serverArr[2].getLocationValue("/images/sample", "file") << std::endl;


    // std::cout << confFile._serverArr[9]<< std::endl;
    
    //std::cout << confFile._serverArr[0].getIp() << std::endl;


/*
    std::string     keyOuter;
    std::map<std::string, std::map<std::string, std::string> >   _locations;

    while (std::getline(file, line))
    {
        if (line.find("server:") != std::string::npos)
            continue;
        else if (line.find("error ") != std::string::npos)
            std::cout << "xxxxx[" << line.substr(15) << "]" << std::endl;
        else if (line.find("server_name: ") != std::string::npos)
        {
            std::cout << "----[" <<line.substr(15) << "]" << std::endl;
            //std::cout << "[" <<line.substr(line.find("server_name:") + std::string("server_name :").length()) << "]" << std::endl;
        } 
        else if (line.find("ip: ") != std::string::npos)
            std::cout << "~~~~[" << line.substr(6) << "]" << std::endl; 
        else if (line.find("root: ") != std::string::npos) 
            std::cout << "oooooooo[" <<line.substr(8) << "]" << std::endl;
        else if (line.find("port: ") != std::string::npos)
            std::cout << "PORT[" <<std::atoi(line.substr(8).c_str()) << "]" << std::endl;
        else if (line.find("body_size: ") != std::string::npos)
            std::cout << "bodysizeeee[" <<std::atoi(line.substr(13).c_str()) << "]" << std::endl;
        else if (line.find("location: ") != std::string::npos)
        {
            keyOuter = line.substr(12);
            _locations[keyOuter];
            std::cout << "-> keys [" << keyOuter << "]" << std::endl;
        }
        else if (line.find("file: ") != std::string::npos)
        {
            _locations[keyOuter]["file"] = line.substr(10);
            std::map<std::string, std::map<std::string, std::string> >::iterator it = _locations.begin();
            std::cout << "Primera key: " << it->first << std::endl;
        }
        else if (line.find("redirect: ") != std::string::npos)
            _locations[keyOuter]["redirect"] = line.substr(14);
        else if (line.find("autoindex: ") != std::string::npos)
            _locations[keyOuter]["autoindex"] = line.substr(15);     
        else if (line.find("GET: ") != std::string::npos)
            _locations[keyOuter]["GET"] = line.substr(11);
        else if (line.find("POST: ") != std::string::npos)
            _locations[keyOuter]["POST"] = line.substr(12);
        else if (line.find("DELETE: ") != std::string::npos)
            _locations[keyOuter]["DELETE"] = line.substr(14);
        else if (line.find("upload_folder: ") != std::string::npos)
            _locations[keyOuter]["upload_folder"] = line.substr(19);
    }
    std::cout << std::endl << "**************" << std::endl << std::endl;
    
    std::cout << "[/] [file] => " << "[" << _locations["/"]["file"] << "]" << std::endl;
    std::cout << "[/] [redirect] => " << "[" << _locations["/"]["redirect"] << "]" << std::endl;
    std::cout << "[/] [autoindex] => " << "[" << _locations["/"]["autoindex"] << "]" << std::endl;
    std::cout << "[/] [GET] => " << "[" << _locations["/"]["GET"] << "]" << std::endl;
    std::cout << "[/] [POST] => " << "[" << _locations["/"]["POST"] << "]" << std::endl;
    std::cout << "[/] [DELETE] => " << "[" << _locations["/"]["DELETE"] << "]" << std::endl;
    std::cout << "[/] [upload_folder] => " << "[" << _locations["/"]["upload_folder"] << "]" << std::endl;

    std::cout << "[/goto42] [file] => " << "[" << _locations["/goto42"]["file"] << "]" << std::endl;
    std::cout << "[/goto42] [redirect] => " << "[" << _locations["/goto42"]["redirect"] << "]" << std::endl;
    std::cout << "[/goto42] [autoindex] => " << "[" << _locations["/"]["autoindex"] << "]" << std::endl;
    std::cout << "[/goto42] [GET] => " << "[" << _locations["/goto42"]["GET"] << "]" << std::endl;
    std::cout << "[/goto42] [POST] => " << "[" << _locations["/goto42"]["POST"] << "]" << std::endl;
    std::cout << "[/goto42] [DELETE] => " << "[" << _locations["goto42"]["DELETE"] << "]" << std::endl;
    std::cout << "[/goto42] [upload_folder] => " << "[" << _locations["/goto42"]["upload_folder"] << "]" << std::endl;

    std::cout << "[/ai] [file] => " << "[" << _locations["/ai"]["file"] << "]" << std::endl;
    std::cout << "[/ai] [redirect] => " << "[" << _locations["/ai"]["redirect"] << "]" << std::endl;
    std::cout << "[/ai] [autoindex] => " << "[" << _locations["/ai"]["autoindex"] << "]" << std::endl;
    std::cout << "[/ai] [GET] => " << "[" << _locations["/ai"]["GET"] << "]" << std::endl;
    std::cout << "[/ai] [POST] => " << "[" << _locations["/ai"]["POST"] << "]" << std::endl;
    std::cout << "[/ai] [DELETE] => " << "[" << _locations["/ai"]["DELETE"] << "]" << std::endl;
    std::cout << "[/ai] [DELETE] => " << "[" << _locations["/ai"]["DELETE"] << "]" << std::endl;
    std::cout << "[/ai] [upload_folder] => " << "[" << _locations["/ai"]["upload_folder"] << "]" << std::endl;

    std::cout << "[/upload] [file] => " << "[" << _locations["/upload"]["file"] << "]" << std::endl;
    std::cout << "[/upload] [redirect] => " << "[" << _locations["/upload"]["redirect"] << "]" << std::endl;
    std::cout << "[/upload] [autoindex] => " << "[" << _locations["/upload"]["autoindex"] << "]" << std::endl;
    std::cout << "[/upload] [GET] => " << "[" << _locations["/upload"]["GET"] << "]" << std::endl;
    std::cout << "[/upload] [POST] => " << "[" << _locations["/upload"]["POST"] << "]" << std::endl;
    std::cout << "[/upload] [DELETE] => " << "[" << _locations["/upload"]["DELETE"] << "]" << std::endl;
    std::cout << "[/upload] [upload_folder] => " << "[" << _locations["/upload"]["upload_folder"] << "]" << std::endl;

    std::cout << "[/favicon.ico] [file] => " << "[" << _locations["/favicon.ico"]["file"] << "]" << std::endl;
    std::cout << "[/favicon.ico] [redirect] => " << "[" << _locations["/favicon.ico"]["redirect"] << "]" << std::endl;
    std::cout << "[/favicon.ico] [autoindex] => " << "[" << _locations["/favicon.ico"]["autoindex"] << "]" << std::endl;
    std::cout << "[/favicon.ico] [GET] => " << "[" << _locations["/favicon.ico"]["GET"] << "]" << std::endl;
    std::cout << "[/favicon.ico] [POST] => " << "[" << _locations["/favicon.ico"]["POST"] << "]" << std::endl;
    std::cout << "[/favicon.ico] [DELETE] => " << "[" << _locations["/favicon.ico"]["DELETE"] << "]" << std::endl;
    std::cout << "[/favicon.ico] [upload_folder] => " << "[" << _locations["/favicon.ico"]["upload_folder"] << "]" << std::endl;
 */
    return (0);


}