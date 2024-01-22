#pragma once

#include "ASocket.hpp"


/* 
    Child class of ASocket
    connectToNetwork function implemented with bind
*/

class BindingSocket: public ASocket
{
    private:

    public:
        BindingSocket(int domain, int service, int protocol, int port, u_long interface);

        int connectToNetwork(int sock, struct sockaddr_in address);

};
