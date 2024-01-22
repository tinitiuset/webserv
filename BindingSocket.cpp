/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BindingSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:06:36 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 17:37:17 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/



#include "BindingSocket.hpp"

BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface):
ASocket(domain, service, protocol, port, interface)
{
    // or int conn = connectToNetwork()...
    _connection = connectToNetwork(_sock, _address);
    testConnection(_connection);
}

int        BindingSocket::connectToNetwork(int sock, struct sockaddr_in address)
{
    return (bind(sock, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)));
}