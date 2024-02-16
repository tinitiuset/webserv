#include "Multiplexer.hpp"
#include "ServerLauncher.hpp"
#include "./model/Conf.hpp"

Conf *conf;

int main() {

  conf = new Conf("./config/webserv.conf");
  conf->parse();

  Multiplexer multiplexer;

  multiplexer.run(ServerLauncher());

  delete conf;
  return 0;
}
