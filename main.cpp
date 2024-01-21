#include "Multiplexer.hpp"
#include "Server.hpp"

int main() {

    Multiplexer().run(Server());

    return 0;
}