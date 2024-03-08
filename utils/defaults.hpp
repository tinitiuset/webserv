#ifndef DEFAULTS_HPP
# define DEFAULTS_HPP

# define BACKLOG 256
# define SERVER_MAX_CONNECTIONS 1024
# define TIMEOUT 10

# include "../model/Conf.hpp"
# include "Logger.hpp"

class Logger;

extern Conf *conf;

#endif
