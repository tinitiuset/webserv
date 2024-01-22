/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 08:59:20 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 11:09:51 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

class ASocket
{
    protected:
        struct  sockaddr_in _address;
        int                 _sock;
        int                 _connection;

    public:
        ASocket(int domain, int service, int protocol, int port, u_long interface);
        virtual ~ASocket();

        //virtual function to connect to a network
        virtual int connectToNetwork(int _sock, struct sockaddr_in address) = 0;
        
        //function to test socket and connections
        void    testConnection(int testSocket);
        
       //getters
       struct sockaddr_in  getAddress();
       int                  getSock();
       int                  getConnection();
       

};
