#include "Multiplexer.hpp"
#include "utils/defaults.hpp"

Conf* conf;

int main() {

  conf = new Conf("./config/webserv.conf");
  conf->parse();
  conf->load();

  Multiplexer().run();

  delete conf;
  return 0;
}
