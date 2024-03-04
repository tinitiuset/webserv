#include "DeleteRequest.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

#include <dirent.h>
#include <sys/stat.h>

int DeleteRequest::delete_file(std::string path){
    if (remove(path.c_str()) != 0)
        return (-1);
    std::cout << "File deleted" << std::endl;
    return (0);    
}

int DeleteRequest::delete_directory(std::string path) {
    DIR* dir = opendir(path.c_str());
    struct dirent* entry;
    char filepath[1024];

    if (!dir)
        return (-1);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (Utils::ft_strcmp(entry->d_name, ".") == 0 || Utils::ft_strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(filepath, sizeof(filepath), "%s/%s", path.c_str(), entry->d_name);
            delete_directory(filepath);
        } else {
            snprintf(filepath, sizeof(filepath), "%s/%s", path.c_str(), entry->d_name);
            remove(filepath);
        }
    }
    closedir(dir);
    remove(path.c_str());
    return (0);
}

DeleteRequest::DeleteRequest(const Request& request): Request(request) {
}

DeleteRequest::~DeleteRequest() {}
std::string DeleteRequest::handle() {

	// AddToList
	// UploadFile
	// SetCookie
	// deChunck

	Logger::debug("DeleteRequest::handle() called");
    Index* loc = dynamic_cast<Index*>(conf->getServer(getPort()).bestLocation(_uri));
    std::string path;
	Response response;
    
    if ((loc->path()).back() == '/')
        path = _uri.replace(0, loc->path().length() - 1, loc->root());
    else
        path = _uri.replace(0, loc->path().length(), loc->root());
    if (loc->isMethodAllowed("delete") == false)
        return (response.format());//Here should respond with error code
    else
    {
        //Check the DELETE target and decide if it is a file or a directory
        if (_uri.back() == '/')
            delete_directory(path);
        else
            delete_file(path);
    }


	response.set_body("File/Directory correctly deleted!");

	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "text/plain"));
	headers.insert(std::make_pair("Content-Length", std::to_string(response.body().length())));


	response.set_headers(headers);

	response.set_start_line("HTTP/1.1 200 OK");

	return response.format();
}
