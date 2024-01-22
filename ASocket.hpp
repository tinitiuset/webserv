/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 08:59:20 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 16:16:14 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "defaults.hpp"

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
       struct sockaddr_in   getAddress();
       int                  getSock();
       int                  getConnection();
       
};
