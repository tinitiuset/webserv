/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:23:16 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 17:44:29 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/



#pragma once

#include "ListeningSocket.hpp"
#include <netinet/in.h>

class Server
{
    private:
        ListeningSocket *_lstSocket;
        int             newSocket;
        char            buffer[1024];

        void            accepter();
        void            handler();
        void            responder();

    public:
        Server(int domain, int service, int protocol, int port, u_long interface, int backlog);
        ~Server();

        void            launch();

        ListeningSocket *getSocket();

};
