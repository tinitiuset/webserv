    /* 
        Si autoindex on
        Si uri es un directorio valido en el server

        Hacemos autoindex en el root/path que figure en el config de la location

        ----

        - cambiamos dir al que pida el root/path de la config de la location
        
        - generamos el html para la cabecera de la pagina autoindex
        - read dir para leer archivos y directorios del directorio actual
            y vamos generando el html con los links a los archivos y directorios
        - devolvemos el dir al de trabajo original / chdir al root del server

        

    */ 


#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP


#include <iostream>
#include <string>
#include <unistd.h>
#include <dirent.h>

class Autoindex
{
    private:
        std::string _uri;
        std::string _root;


    public:
        Autoindex(std::string uri, std::string root);

};


#endif

