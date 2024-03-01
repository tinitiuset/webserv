#include "DeleteRequest.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

#include <dirent.h>
#include <sys/stat.h>

void remove_directory(const char* path) {
    DIR* dir = opendir(path);
    struct dirent* entry;
    char filepath[1024];

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
            remove_directory(filepath);
        } else {
            snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
            remove(filepath);
        }
    }
    closedir(dir);
    remove(path);
}

DeleteRequest::DeleteRequest(const Request& request): Request(request) {
}

DeleteRequest::~DeleteRequest() {}
std::string DeleteRequest::handle() {

	// AddToList
	// UploadFile
	// SetCookie
	// deChunck

	Logger::debug("PostRequest::handle() called");

	delete_file(_body);

	Response response;

	response.set_body("File correctly uploaded!");

	std::map<std::string, std::string> headers;
	headers.insert(std::make_pair("Content-Type", "text/plain"));
	headers.insert(std::make_pair("Content-Length", std::to_string(response.body().length())));


	response.set_headers(headers);

	response.set_start_line("HTTP/1.1 200 OK");

	return response.format();
}

//Method to save the file in the server
void	DeleteRequest::delete_file(std::string body){
	
}