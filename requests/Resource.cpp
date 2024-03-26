#include "Resource.hpp"

Resource::Resource(std::string path, std::string method) {
	Logger::debug("Resource::Resource() creating resource with path " + path);
	_status = 501;
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

Resource::~Resource() {
}

std::string Resource::load() {
	Logger::info("Resource::load() Loading resource from " + _path);

	std::ifstream file;

	(mime().find("text") == std::string::npos) ?
		file.open(_path.c_str(), std::ios::binary) : file.open(_path.c_str());

	if (!file.is_open() || Utils::isDirectory(_path.c_str())) {
		throw RequestException(404);
	}

	try {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		_status = 200;
		return content;
	} catch (const std::exception& e) {
		throw RequestException(500);
	}
}

std::string Resource::status() const {
	return "HTTP/1.1 " + Codes::status(_status);
}

std::string Resource::mime() const {

	std::map<std::string, std::string> mimeTypes;

	mimeTypes["html"] = "text/html";
	mimeTypes["htm"] = "text/html";
	mimeTypes["css"] = "text/css";
	mimeTypes["js"] = "text/javascript";
	mimeTypes["jpg"] = "image/jpeg";
	mimeTypes["jpeg"] = "image/jpeg";
	mimeTypes["png"] = "image/png";
	mimeTypes["gif"] = "image/gif";
	mimeTypes["ico"] = "image/x-icon";
	mimeTypes["svg"] = "image/svg+xml";
	mimeTypes["pdf"] = "application/pdf";
	mimeTypes["json"] = "application/json";
	mimeTypes["xml"] = "application/xml";
	mimeTypes["mp3"] = "audio/mpeg";
	mimeTypes["mp4"] = "video/mp4";
	mimeTypes["webm"] = "video/webm";
	mimeTypes["ogg"] = "audio/ogg";
	mimeTypes["wav"] = "audio/wav";
	mimeTypes["avi"] = "video/x-msvideo";
	mimeTypes["mpeg"] = "video/mpeg";
	mimeTypes["txt"] = "text/plain";

	std::string extension = _path.substr(_path.find_last_of('.') + 1);
	if (mimeTypes.find(extension) != mimeTypes.end())
		return mimeTypes[extension];
	return "text/plain";
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
		Logger::error("Resource::buildAI() opendir() failed");
		throw RequestException(500);;
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
	{
        Logger::error("Resource::extractCgi() / not found in extractCgi method");
		throw RequestException(400);;
	}

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
	{
		Logger::error("Resource::extractQStr() ? not found in extractQStr method");
		throw RequestException(400);;
	}
	std::string qStr = _path.substr(questionMarkPos + 1);

	return qStr;
}

std::string Resource::buildCGI(std::string qStr)
{
	std::string cgiPath = extractCgi();
	std::string interpret = "";
	_env = NULL;

	if (access(_path.c_str(), X_OK))
	{
		Logger::info("Resource::buildCGI() access() failed");
		throw RequestException(403);
	}

	if (cgiPath.substr(cgiPath.length() - 3) == ".py")
        interpret = "/usr/local/bin/python3";
    else if (_path.substr(_path.length() - 3) == ".pl")
        interpret = "/usr/bin/perl";
    else
	{
		Logger::info("Resource::buildCGI() invalid file extension");
		throw RequestException(403);
	}

	return (initCgi(interpret, qStr));
}

std::string    Resource::initCgi(std::string interpret, std::string qStr)
{
	int	fd_parent_to_child[2];
    int fd_child_to_parent[2];

	if (pipe(fd_child_to_parent) == -1)
	{
		Logger::error("Resource::initCgi() pipe() failed");
        throw RequestException(500);
	}
    
    if (_method == "GET") //also check if method is supported in config????
        set4GETEnv(_path, qStr);
    else if (_method == "POST") //also check if method is supported in config????
    {
		if (pipe(fd_parent_to_child) == -1)
		{
			Logger::error("Resource::initCgi() pipe() failed");
			throw RequestException(500);
		}
		set4Post();
	}
    else
		throw RequestException(405);

    pid_t pid = fork();

    if (pid == -1)
	{
		Logger::error("Resource::initCgi() fork() failed");
		throw RequestException(500);
	}

    else if (pid == 0) //child process
    {
        if (_method == "POST")
		{
			close(fd_parent_to_child[1]);
			if (dup2(fd_parent_to_child[0], STDIN_FILENO) == -1)
			{
				Logger::error("Resource::initCgi() dup2() failed");
				throw RequestException(500);
			}
		}
		close(fd_child_to_parent[0]);
		if (dup2(fd_child_to_parent[1], STDOUT_FILENO) == -1)
		{
			Logger::error("Resource::initCgi() dup2() failed");
			throw RequestException(500);
		}
		close(fd_child_to_parent[1]);
		if (_method == "POST")
			close(fd_parent_to_child[0]);

        char *args[3];
		args[0] = (char *)interpret.c_str();
        args[1] = (char *)_path.c_str();
        args[2] = NULL;

	    if (execve(interpret.c_str(), args, _env) == -1)
		{
			delete _env;
			Logger::error("Resource::initCgi() execve() failed");
            throw RequestException(500);    
		}
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
		if (current_time - start_time > TIMEOUT)
		{
			Logger::error("Resource::initCgi() Timeout. Killing child process");
			kill(pid, SIGKILL);
			delete _env;
			throw RequestException(408);
		}
		usleep(100000);  // (0.1 sec)
	}
	delete _env;
	_status = 200;
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
		{
			Logger::error("Resource::readChildOutput() read() failed");
			throw RequestException(500);
		}
		buffer[bytes] = '\0';
		resp += buffer;
	}
	close(fd_child_to_parent);
	return(resp);
}

void    Resource::set4GETEnv(std::string cgiPath, std::string qStr)
{
    _env = new char*[8];

    std::vector<std::string>    envVect;

	envVect.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envVect.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envVect.push_back("REQUEST_METHOD=" + _method);
	envVect.push_back("PATH_INFO=" + cgiPath);
    envVect.push_back("SCRIPT_NAME=" + cgiPath);
    envVect.push_back("QUERY_STRING=" + qStr);
    envVect.push_back("CONTENT_LENGTH=1024");

    for (int i = 0; i < 7; i++)
        _env[i] = (char *)envVect[i].c_str();
    _env[7] = NULL;
}

void    Resource::set4Post()
{
    _env = new char*[4];

    std::vector<std::string>    envVect;

    envVect.push_back("REQUEST_METHOD=" + _method);
    envVect.push_back("CONTENT_LENGTH=1024");
	envVect.push_back("PATH_INFO=" + _path);
    for (int i = 0; i < 3; i++)
        _env[i] = (char *)envVect[i].c_str();
    _env[3] = NULL;
}
