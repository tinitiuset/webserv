/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 12:06:48 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 18:41:30 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once

#include "BindingSocket.hpp"
#include <netinet/in.h>

class ListeningSocket: public BindingSocket
{
    private:
        int _backlog;
        int _listen;
    public:
        ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog);
        
        void    startListening();

        int     getBacklog();
};
