/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imontero <imontero@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:24:35 by imontero          #+#    #+#             */
/*   Updated: 2024/01/22 17:46:42 by imontero         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Server.hpp"
#include "defaults.hpp"
#include <netinet/in.h>


int main ()
{
    Server test(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10);

    std::cout << "Agures" << std::endl;
}