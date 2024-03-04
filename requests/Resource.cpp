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
	std::string extension = _path.substr(_path.find_last_of(".") + 1);
	if (mime().find("text") == std::string::npos) {
		std::ifstream file(_path, std::ios::binary);
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		return content;
	} else {
		std::ifstream file(_path);
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		return content;
	}
}

std::string Resource::status() const {
	return _status;
}

std::string Resource::mime() const {
	std::string extension = _path.substr(_path.find_last_of(".") + 1);
	std::string mime = "text/plain";
	if (extension == "html" || extension == "htm")
		mime = "text/html";
	else if (extension == "css")
		mime = "text/css";
	else if (extension == "js")
		mime = "text/javascript";
	else if (extension == "jpg" || extension == "jpeg")
		mime = "image/jpeg";
	else if (extension == "png")
		mime = "image/png";
	else if (extension == "gif")
		mime = "image/gif";
	else if (extension == "ico")
		mime = "image/x-icon";
	else if (extension == "svg")
		mime = "image/svg+xml";
	else if (extension == "pdf")
		mime = "application/pdf";
	else if (extension == "json")
		mime = "application/json";
	else if (extension == "xml")
		mime = "application/xml";
	else if (extension == "mp3")
		mime = "audio/mpeg";
	else if (extension == "mp4")
		mime = "video/mp4";
	else if (extension == "webm")
		mime = "video/webm";
	else if (extension == "ogg")
		mime = "audio/ogg";
	else if (extension == "wav")
		mime = "audio/wav";
	else if (extension == "avi")
		mime = "video/x-msvideo";
	else if (extension == "mpeg")
		mime = "video/mpeg";
	else if (extension == "txt")
		mime = "text/plain";
	return mime;
}

std::string Resource::buildAI(std::string uri, std::string host, std::string resPath)
{
    std::string result = "";
		
	DIR *dir;
    struct dirent *ent;

	result += "<html>\n<head>\n<title>Index of ";
	result += uri + "</title>\n";
	result += "</head>\n<body>\n<h1>Index of ";
	result += uri;
	result += "</h1>\n<ul>\n";

	std::string prevUri = getPreviousUri(uri);

    result += "<li>\n<a href=\"http://" + host + prevUri + "\">" + ".." + "</a>\n</li>\n";

    if ((dir = opendir(resPath.c_str())) != NULL)
	{
        while ((ent = readdir(dir)) != NULL) 
		{
            std::string	file(ent->d_name);
			if (file != "." && file != "..")
				result += "<li>\n<a href=\"http://" + host + uri + "/" + file + "\">" + file + "</a>\n</li>\n";
        }
        closedir(dir);
    }
	else
	{
        std::cerr << "No se pudo abrir el directorio" << std::endl;
        return ("");
    }
	result += "</ul>\n</body>\n</html>\n";

	std::cout << "Result: " << result << std::endl << std::endl;

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
