#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

class ASocket
{
    private:
        struct  sockaddr_in address;
        int     sock;
        int     connection;

    public:
        ASocket();
        ASocket(int domain, int service, int protocol, int port, u_long interface);
        ASocket(ASocket const & inst);        
        ~ASocket();

        ASocket & operator =(ASocket & const rhs);

        //virtual function to connect to a network
        virtual int connectToNetwork(int sock, struct sockaddr_in address) = 0;

       //getters
       struct sockaddre_in  get_address();
       int                  get_sock();
       int                  get_connection();
       

};
