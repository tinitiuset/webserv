#include "Multiplexer.hpp"
#include "utils/defaults.hpp"
#include <string>

Conf* conf;

int main() {

  conf = new Conf("./config/webserv.conf");
  
  conf->parse();
  conf->load();

  Multiplexer().run();

  delete conf;
  return 0;
}