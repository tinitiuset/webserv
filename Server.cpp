/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:23:08 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 18:36:17 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Server.hpp"
//#include "ListeningSocket.hpp"
#include <unistd.h>

Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog)
{
    _lstSocket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
    launch();
}

Server::~Server()
{
    
    delete _lstSocket;
    
}

void            Server::accepter()
{
    struct sockaddr_in  address = _lstSocket->getAddress();
    int                 addrlen = sizeof(address);
    newSocket = accept(_lstSocket->getSock(), reinterpret_cast<struct sockaddr*>(&address), (socklen_t *)&addrlen);
    if (read(newSocket, buffer, 1024) < 0)
    {
        std::cerr << "Error reading..." << std::endl;
        exit (-1);
    }
}

void            Server::handler()
{
    std::cout << "🤖 Message arrived to server: " << std::endl << buffer << std::endl;
}

void            Server::responder()
{
    std::cout << "Responder from server..." << std::endl;
}

ListeningSocket *Server::getSocket()
{
    return (_lstSocket);
}

void    Server::launch()
{
    while (true)
    {
        std::cout << "⏳...Waiting..." << std::endl;
        accepter();
        handler();
        std::cout << "😎 This is server, mthrfckr" << std::endl;
        
    }
}
