/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 08:59:05 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 11:17:46 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ASocket.hpp"

ASocket::ASocket(int domain, int service, int protocol, int port, u_long interface)
{
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    _sock = socket(domain, service, protocol);
    testConnection(_sock);
        
}

ASocket::~ASocket() {}

void                ASocket::testConnection(int testSocket)
{
    if (testSocket < 0)
    {
        std::cerr << "Failed to connect..." << std::endl;
        exit(-1);
    }
    std::cout << "testConnection output is " << testSocket << std::endl;
}

struct sockaddr_in  ASocket::getAddress()
{
    return (_address);
}

int                  ASocket::getSock()
{
    return (_sock);
}


int                  ASocket::getConnection()
{
    return (_connection);
}