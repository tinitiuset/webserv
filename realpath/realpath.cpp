#include <iostream>
#include "../utils/Utils.hpp"

/*
  server {
  listen 0.0.0.0:8050;
  server_name example.com;
  body_size 1000;
  root /var/www/html;

  location /redirect {
    redirect 301 /redirected;
  }

  location / {
    root /var/www/html;
    autoindex on;
    methods get post;
  }

  location /favicon {
    index index.html;
    cgi on;
    root /var/www/html;
  }
*/

/* 
	build and returns the correct path
*/
std::string buildRealPath(std::string uri, std::string location, std::string root, std::string &file)
{
	location = Utils::removeLastSlash(location);
	uri = Utils::removeLastSlash(uri);
	root = Utils::removeLastSlash(root);

	if (uri == location)
		return (root + "/" + file);
	return (Utils::strReplace(uri, location, root));
}

int main()
{
    std::string uri = "/favicon/pepe/";

    std::string location = "/favicon";
    std::string root = "/var/www/html/";
    std::string file = "index.html";

	std::cout << buildRealPath(uri, location, root, file) << std::endl;

    return 0;
}