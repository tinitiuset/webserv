/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectingSocket.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:06:46 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 12:18:35 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once

#include "ASocket.hpp"

/* 
    Child class of ASocket
    connectToNetwork function implemented with connect
*/

class ConnectingSocket: public ASocket
{
    private:

    public:
        ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);

        int connectToNetwork(int sock, struct sockaddr_in address);

};
