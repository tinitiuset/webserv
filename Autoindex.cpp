#include "Autoindex.hpp"


Autoindex::Autoindex(std::string serverName, std::string locName, int port, std::string root, std::string uri):
_uri(uri), _locName(locName)
{
    _host = serverName + ":" + Utils::toString(port);
    _serverPath = "." + root;


}

std::string Autoindex::buildAI()
{
    std::string result = "";
	
	
	DIR *dir;
    struct dirent *ent;

    //result += "HTTP/1.1 200 OK\n\n";

	result += "<html>\n<head>\n<title>Index of ";
	result += _uri + "</title>\n";
	result += "</head>\n<body>\n<h1>Index of ";
	result += _uri;
	result += "</h1>\n<ul>\n";

	//result += "<li>\n<a href=\"" + _serverPath + _uri + "/" + "..\">" + ".." + "</a>\n</li>\n";

    std::string path = Utils::strReplace(_uri, _locName, _serverPath);
    if ((dir = opendir(path.c_str())) != NULL)
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
    if (archivo.is_open()) {
        archivo << result;
        archivo.close();
        std::cout << "Se ha escrito el contenido en el archivo output.html" << std::endl;
    } else {
        std::cerr << "No se pudo abrir el archivo para escritura" << std::endl;
        return ("");
    }

	return (result);
}

