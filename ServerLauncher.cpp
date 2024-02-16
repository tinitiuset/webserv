#include "ServerLauncher.hpp"

ServerLauncher::ServerLauncher(){
	
  for (int i = 0; i < conf->getServerCount(); i++) {
    sockfd.push_back(-1);
    socketOps(conf->getServer(i).port(), i);
	}
}

ServerLauncher::~ServerLauncher() {
  Logger::info("Server shutting down");

  for (size_t i = 0; i < sockfd.size(); ++i)
      close(sockfd[i]);
}

int ServerLauncher::Accept(size_t i) const {

  sockaddr_in cli = {};
  socklen_t len = sizeof(cli);

  // https://man7.org/linux/man-pages/man2/accept.2.html
  const int connfd =
      accept(sockfd[i], reinterpret_cast<struct sockaddr *>(&cli), &len);
  (connfd < 0) ? throw std::runtime_error("Server accept failed\n")
               : Logger::debug("Server accepted the client");
  return connfd;
}

std::vector<int> ServerLauncher::getSocketFd() const { return sockfd; }

void ServerLauncher::socketOps(int port, int i) {
  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  try {
    if (port <= 0 || port > 65535)
      throw std::runtime_error("Invalid port number\n");
    servaddr.sin_port = htons(port);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      ? throw std::runtime_error("Socket creation failed\n")
      : Logger::debug("Socket successfully created");

  // set socket as reusable

  int option = 1;
  ((setsockopt(sockfd[i], SOL_SOCKET, SO_REUSEADDR, &option,
               sizeof(option)) == -1))
      ? throw std::runtime_error("Setting to reusable failed\n")
      : Logger::debug("Socket options successfully setted");

  // https://man7.org/linux/man-pages/man2/bind.2.html
  (bind(sockfd[i], reinterpret_cast<struct sockaddr *>(&servaddr),
        sizeof(servaddr)) == -1)
      ? throw std::runtime_error("Binding failed\n")
      : Logger::debug("Socket successfully binded");

  // https://man7.org/linux/man-pages/man2/listen.2.html
  try {
    listen(sockfd[i], BACKLOG);
    // Logger::info("Server listening on port " + std::to_string(port)); //OJO
    // to_string cpp11?
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  // setting socket as non blocking
  (fcntl(sockfd[i], F_SETFL, O_NONBLOCK) == -1)
      ? throw std::runtime_error("Setting to nonblocking failed")
      : Logger::debug("Socket setted to non blocking");

  Logger::info("Server " + std::to_string(i) + " listening in port " +
               std::to_string(port));
}

void checkRepeatedPorts(const std::vector<int> &vector) {
  for (size_t i = 0; i < vector.size(); ++i)
    for (size_t j = i + 1; j < vector.size(); ++j)
      if (vector[i] == vector[j])
        throw std::runtime_error("Repeated ports in config file\n");
}
