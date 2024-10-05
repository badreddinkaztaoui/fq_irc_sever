/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:39:06 by nben-ais          #+#    #+#             */
/*   Updated: 2024/06/02 16:13:41 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/IrcServ.hpp"

int main(int ac, char **av) {

    if (ac != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        exit (1);
    }

    try {
        std::string port = av[1];
        std::string password = av[2];

        std::cout << "Server started on port " << port << " with password " << password << std::endl;

        IrcServ server(port, password);
        server.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}