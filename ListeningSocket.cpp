/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:06:31 by imontero          #+#    #+#             */
/*   Updated: 2024/01/26 09:47:49 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "ListeningSocket.hpp"
#include <unistd.h>

ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog):
BindingSocket(domain, service, protocol, port, interface),
_backlog(backlog)
{
    startListening();
    testConnection(_listen);
}

void    ListeningSocket::startListening()
{
    _listen = listen(_sock, _backlog);
}

int     ListeningSocket::getBacklog()
{
    return (_backlog);
}