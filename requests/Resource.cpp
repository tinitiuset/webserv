#include "Resource.hpp"

Resource::Resource(std::string path, std::string method) {
	Logger::debug("Resource::Resource() creating resource with path " + path);
	_status = "HTTP/1.1 200 OK";
	_path = path;
	_method = method;
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

/* 
	extract the name of the cgi script from the path until "?"
	"/var/www/html/cgi-bin/hello.py?name=pepe" returns "hello.py"

*/
std::string Resource::extractCgi()
{
	size_t lastSlashPos = _path.rfind('/');
    if (lastSlashPos == std::string::npos)
        return "";

    std::string cgiFileName = _path.substr(lastSlashPos + 1);

    size_t questionMarkPos = cgiFileName.find('?');
    if (questionMarkPos != std::string::npos)
        cgiFileName = cgiFileName.substr(0, questionMarkPos);

    return cgiFileName;
}

std::string Resource::extractQStr()
{
	size_t questionMarkPos = _path.find('?');
	if (questionMarkPos == std::string::npos)
		return "";

	std::string qStr = _path.substr(questionMarkPos + 1);

	return qStr;
}

std::string Resource::buildCGI()
{
	std::string cgiPath = extractCgi();
	std::string qStr = extractQStr();
	std::string interpret = "";
	_env = NULL;
	
	if (cgiPath.substr(cgiPath.length() - 3) == ".py")
        interpret = "/usr/local/bin/python3";
    else if (_path.substr(_path.length() - 3) == ".pl")
        interpret = "/usr/bin/perl";
    else
        throw std::runtime_error("invalid cgi script");

	initCgi(cgiPath, interpret, qStr);
}

std::string    Resource::initCgi(std::string cgiPath, std::string interpret, std::string qStr)
{
	int	fd_parent_to_child[2];
    int fd_child_to_parent[2];

	if (pipe(fd_child_to_parent) == -1)
        throw std::runtime_error("pipe error");
    
    if (_method == "GET") //also check if method is supported in config????
        set4GETEnv(cgiPath, qStr);
    else if (_method == "POST") //also check if method is supported in config????
    {
		if (pipe(fd_parent_to_child) == -1)
			throw std::runtime_error("pipe error");
		set4Post();
	}
    else
    {
        std::cerr << "method not supported" << std::endl;
        return (""); //?
    }

    pid_t pid = fork();

    if (pid == -1)
        throw std::runtime_error("fork error");
    else if (pid == 0) //child process
    {
        if (_method == "POST")
		{
			close(fd_parent_to_child[1]);
			dup2(fd_parent_to_child[0], STDIN_FILENO);   
		}
		close(fd_child_to_parent[0]);
		dup2(fd_child_to_parent[1], STDOUT_FILENO);
		close(fd_child_to_parent[1]);
		if (_method == "POST")
			close(fd_parent_to_child[0]);

        char *args[3];
		args[0] = (char *)interpret.c_str();
        args[1] = (char *)cgiPath.c_str();
        args[2] = NULL;

        if (execve(interpret.c_str(), args, _env) == -1)
            throw std::runtime_error("execve error");    
    }
	//parent process
	if (_method == "POST")
	{
		write(fd_parent_to_child[1], qStr.c_str(), qStr.length());
		close(fd_parent_to_child[0]);
		close(fd_parent_to_child[1]);
	}
	close(fd_child_to_parent[1]);
	
	std::time_t start_time = std::time(NULL);
	int status;
	while (waitpid(pid, &status, WNOHANG) == 0)
	{
		std::time_t current_time = std::time(NULL);
		if (current_time - start_time > TIMEOUT)  // Ajusta el límite de tiempo según tus necesidades
		{
			std::cout << "Timeout. Killing child process" << std::endl;
			kill(pid, SIGKILL);
			return ("");
		}
		usleep(100000);  // (0.1 sec)
	}
	delete _env;
	return (readChildOutput(fd_child_to_parent[0]));
}

std::string     Resource::readChildOutput(int fd_child_to_parent)
{
	char   buffer[4056];
	std::string resp;
	ssize_t bytes;
	while ((bytes = read(fd_child_to_parent, buffer, sizeof(buffer))) > 0)
	{
		if (bytes < 0)
			throw std::runtime_error("fd read error");
		buffer[bytes] = '\0';
		resp += buffer;
	}
	close(fd_child_to_parent);
	return(resp);
}

void    Resource::set4GETEnv(std::string cgiPath, std::string qStr)
{
    _env = new char*[7];

    std::vector<std::string>    envVect;

    envVect.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envVect.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envVect.push_back("REQUEST_METHOD=" + _method);
    if (access(cgiPath.c_str(), X_OK))
        throw std::runtime_error("invalid script or not found");
    envVect.push_back("SCRIPT_NAME=" + cgiPath);
    envVect.push_back("QUERY_STRING=" + qStr);
    envVect.push_back("CONTENT_LENGTH=1024");

    for (int i = 0; i < 7; i++)
        _env[i] = (char *)envVect[i].c_str();
    _env[7] = NULL;
}

void    Resource::set4Post()
{
    _env = new char*[3];

    std::vector<std::string>    envVect;

    envVect.push_back("REQUEST_METHOD=" + _method);
    envVect.push_back("CONTENT_LENGTH=1024");

    for (int i = 0; i < 3; i++)
        _env[i] = (char *)envVect[i].c_str();
    _env[3] = NULL;
}