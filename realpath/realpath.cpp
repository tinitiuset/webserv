#include <iostream>

/* server {
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
  } */

int main()
{
    std::string uri = "/favicon";

    std::string location = "/favicon";
    std::string root = "/var/www/html";
    std::string file = "index.html";

    


    return 0;
}