#include "Multiplexer.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Location.hpp"
#include "PostRequest.hpp"

#include <stdlib.h>
#include <fcntl.h>
#include <vector>

int main() {
    std::vector<Location> locations;

    locations.push_back(Location("/test"));
    locations.push_back(Location("/home"));
    locations.push_back(Location("/test/www"));
    locations.push_back(Location("/test/www/html"));
    locations.push_back(Location("/"));

    int fd = open("test.txt", O_RDWR | O_CREAT, 0666);
    Request req(fd, locations);
    Request *postReq = new PostRequest(req);
    postReq->handle();
    cllo
    delete (postReq);
    return 0;
}