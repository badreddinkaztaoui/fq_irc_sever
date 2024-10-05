/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 15:43:31 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/14 16:05:32 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"
#include "../../include/Utils.hpp"

std::string IrcServ::connect(Request request, int fd) {
    if (request.args.size() != 3)
        return "\033[1;31m\tUsage: /CONNECT <hostname> <port> <password>\033[0m\n";
    if (clientList[fd])
        return "\033[1;31m\tYou're already connected\033[0m\n";
    if (request.args[0] != "localhost" || request.args[1] != port_ || request.args[2] != passWord_)
        return "\033[1;31m\tCan't connect to the server\033[0m\n";
    clientList[fd] = new Client(fd);
    connectedMsg(fd);
    return "";
}