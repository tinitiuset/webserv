#include "Resource.hpp"

Resource::Resource(std::string path) {
	Logger::debug("Resource::Resource() creating resource with path " + path);
	_status = "HTTP/1.1 200 OK";
	_path = path;
}

Resource::Resource(const Resource& resource) {
	_path = resource._path;
}

Resource& Resource::operator=(const Resource& resource) {
	_path = resource._path;
	return *this;
}

Resource::~Resource() {}

std::string Resource::load() const {
	Logger::info("Resource::load() Loading resource from " + _path);
	std::ifstream file(_path);
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

std::string Resource::status() const {
	return _status;
}

std::string Resource::buildAI(std::string uri, int port, std::string address, std::string resPath)
{
    std::string result = "";
	std::string host = "localhost:" + Utils::toString(port);
    resPath = "." + resPath;
	
	DIR *dir;
    struct dirent *ent;

	result += "<html>\n<head>\n<title>Index of ";
	result += uri + "</title>\n";
	result += "</head>\n<body>\n<h1>Index of ";
	result += uri;
	result += "</h1>\n<ul>\n";

	std::string prevUri = getPreviousUri(uri);
    result += "<li>\n<a href=\"localhost" + prevUri + "\">" + ".." + "</a>\n</li>\n";

    //std::string path = Utils::strReplace(_uri, _locName, _serverPath);
    std::cout << resPath << std::endl;
    //if ((dir = opendir(resPath.c_str())) != NULL)
    if (Utils::isDirectory(resPath.c_str()) && (dir = opendir(resPath.c_str())) != NULL)
	{
        while ((ent = readdir(dir)) != NULL) 
		{
            std::string	file(ent->d_name);
			if (file != "." && file != "..")
				result += "<li>\n<a href=\"" + host + uri + "/" + file + "\">" + file + "</a>\n</li>\n";
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

	return (result);
}

/*
    erase the last folder in uri and returns a string for ".." link
    "/var/www/" becomes "/var"
    "/var/www/html" becomes "var/www"
    "/" doesn´t change
*/
std::string Resource::getPreviousUri(std::string uri)
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
