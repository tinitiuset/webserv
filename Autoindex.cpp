#include "Autoindex.hpp"


Autoindex::Autoindex(int servIdx, int locationIdx, std::string serverName, std::string locName, int port, std::string root, std::string uri):
_servIdx(servIdx), _locationIdx(locationIdx), _locName(locName)
{
    size_t pos = uri.find_last_of("/");
    size_t len = uri.length();
    if (pos != std::string::npos && pos == len - 1 && len > 1)
        uri.erase(pos);
    
    _uri = uri;
    
    _host = serverName + ":" + Utils::toString(port);
    _serverPath = "." + root;
    _pathReplaced = Utils::strReplace(_uri, _locName, _serverPath);

    std::cout << _host << std::endl;
    std::cout << _serverPath << std::endl;
    std::cout << _pathReplaced << std::endl;

}

/* 
    Checks if:
        * uri is valid
        * autoindex is on
        * it´s a directory (not a file)
    returns (true)
*/
bool    Autoindex::isAI()
{
    bool is = true;

    if (!Utils::isDirectory(_pathReplaced.c_str()) ||
            Utils::isFile(_pathReplaced.c_str()))
        is = false;

    /* if (configFile.server[_servIdx].location[_locationIdx].autoIndex == false)
        is = false; */

    return (is);
}


/*
    Returns a html with the list of indexes asked by the uri
*/
std::string Autoindex::buildAI()
{
    std::string result = "";
	
	
	DIR *dir;
    struct dirent *ent;

	result += "<html>\n<head>\n<title>Index of ";
	result += _uri + "</title>\n";
	result += "</head>\n<body>\n<h1>Index of ";
	result += _uri;
	result += "</h1>\n<ul>\n";

	std::string prevUri = getPreviousUri(_uri);
    result += "<li>\n<a href=\"" + _host + prevUri + "\">" + ".." + "</a>\n</li>\n";

    //std::string path = Utils::strReplace(_uri, _locName, _serverPath);
    std::cout << _pathReplaced << std::endl;
    if ((dir = opendir(_pathReplaced.c_str())) != NULL)
	{
        while ((ent = readdir(dir)) != NULL) 
		{
            std::string	file(ent->d_name);
			if (file != "." && file != "..")
				result += "<li>\n<a href=\"" + _host + _uri + "/" + file + "\">" + file + "</a>\n</li>\n";
        }
        closedir(dir);
    }
	else
	{
        std::cerr << "No se pudo abrir el directorio" << std::endl;
        return ("");
    }
	result += "</ul>\n</body>\n</html>\n";

	std::cout << result << std::endl;


	std::ofstream archivo("output.html");
    if (archivo.is_open())
    {
        archivo << result;
        archivo.close();
        std::cout << "Se ha escrito el contenido en el archivo output.html" << std::endl;
    }
    else
    {
        std::cerr << "No se pudo abrir el archivo para escritura" << std::endl;
        return ("");
    }

	return (result);
}

/*
    erase the last folder in uri and returns a string for ".." link
    "/var/www/" becomes "/var"
    "/var/www/html" becomes "var/www"
    "/" doesn´t change
*/
std::string Autoindex::getPreviousUri(std::string uri)
{
    size_t pos = uri.find_last_of("/");
    size_t len = uri.length();
    
    if (pos != std::string::npos && pos == len - 1 && len > 1)
        uri.erase(pos);

    std::string res = uri;

    pos = uri.find_last_of("/");

    if (pos != std::string::npos && pos != 0 && pos != uri.size() - 1)
        uri.erase(pos);

    if (res == uri)
        uri = "/";

    return (uri);
}
