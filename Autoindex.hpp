

#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include "defaults.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include "Utils.hpp"

    /* 
        En location:
        
        Si autoindex on
        &&
        Si uri es un directorio (no archivo) valido en el server

            Devolvemos html con autoindex en el root/path
            que figure en el config de la location

        ----

        - cambiamos dir al que pida el root/path de la config de la location
        - generamos el html para la cabecera de la pagina autoindex
        - read dir para leer archivos y directorios del directorio actual
            y vamos generando el html con los links a los archivos y directorios
        - devolvemos el dir al de trabajo original / chdir al root del server
    */

class Autoindex
{
    private:
        std::string _serverPath;
        std::string _uri;
        std::string _locName;

        std::string _host;


    public:
        Autoindex(std::string serverName, std::string locName, int port, std::string root, std::string uri);

        std::string  buildAI();


};


#endif

