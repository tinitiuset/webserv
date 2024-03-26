#include "DeleteRequest.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

#include <dirent.h>
#include <sys/stat.h>

void DeleteRequest::delete_file(std::string path){
    if (access(path.c_str(), F_OK) != 0)
    {
        throw RequestException(404);
        return;
    }
    if (remove(path.c_str()) != 0)
    {
        throw RequestException(403);
        return;
    }
    std::cout << "File deleted" << std::endl;
    return;    
}

void DeleteRequest::delete_directory(std::string path) {
    DIR* dir = opendir(path.c_str());
    if (!dir)
    {
        throw RequestException(404);
        return;
    }
    struct dirent* entry;
    char filepath[1024];
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (Utils::ft_strcmp(entry->d_name, ".") == 0 || Utils::ft_strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(filepath, sizeof(filepath), "%s/%s", path.c_str(), entry->d_name);
            delete_directory(filepath);
        } else {
            snprintf(filepath, sizeof(filepath), "%s/%s", path.c_str(), entry->d_name);
            DeleteRequest::delete_file(filepath);
        }
    }
    closedir(dir);
    remove(path.c_str());
    return;
}

DeleteRequest::DeleteRequest(const Request& request): Request(request) {
}

DeleteRequest::~DeleteRequest() {}

void DeleteRequest::handle() {

	Logger::debug("DeleteRequest::handle() called");
    Index* loc = dynamic_cast<Index*>(conf->getServer(getPort()).bestLocation(_uri));
    std::string path;
    
	Response response;
    try{
        Request::hostnameAllowed();
        Request::methodAllowed();
        if ((loc->path())[(loc->path()).length() - 1] == '/')
            path = _uri.replace(0, loc->path().length() - 1, loc->root());
        else
            path = _uri.replace(0, loc->path().length(), loc->root());
        
        //Check the DELETE target and decide if it is a file or a directory
        if (_uri[_uri.length() - 1] == '/')
            delete_directory(path);
        else
            delete_file(path);


        response.set_body("File/Directory correctly deleted!");

        std::map<std::string, std::string> headers;
        headers.insert(std::make_pair("Content-Type", "text/plain"));
        headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));


        response.set_headers(headers);

        response.set_start_line("HTTP/1.1 200 OK");
    } catch (const RequestException& exception) {
        response.set_start_line("HTTP/1.1 " + Codes::status(exception.status()));
		response.set_body(ErrorPage::build(exception.status()));
		std::map<std::string, std::string> headers;
		headers.insert(std::make_pair("Content-Type", "text/html"));
		headers.insert(std::make_pair("Content-Length", Utils::toString(response.body().length())));
		response.set_headers(headers);
    }
	_raw = response.format();
}
