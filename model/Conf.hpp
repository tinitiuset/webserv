#ifndef CONF_HPP
#define CONF_HPP
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Server.hpp"

class Conf {

private:
  std::string _file;
  std::vector<Server> _servers;

public:
  Conf(const std::string &file);
  ~Conf();

  void parse();
  void load();

  Server getServer(int index);
  int getServerCount();
  std::vector<int> getServerSockets();
};

#endif
