/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectingSocket.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:06:15 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 12:06:16 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/



#include "ConnectingSocket.hpp"

ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface):
ASocket(domain, service, protocol, port, interface)
{
    // or int conn = connectToNetwork()...
    _connection = connectToNetwork(_sock, _address);
    testConnection(_connection);
}

ConnectingSocket::~ConnectingSocket() {}

int        ConnectingSocket::connectToNetwork(int sock, struct sockaddr_in address)
{
    return (connect(sock, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)));
}