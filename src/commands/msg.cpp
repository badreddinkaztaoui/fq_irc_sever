/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 04:22:11 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/19 15:38:14 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"

std::string IrcServ::msg(Request request, int fd) {
    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected to the server\033[0m\n";
    if (request.args.size() < 1)
        return "\033[1;31m\tUsage: /MSG <message>\033[0m\n";
    if (clientList[fd]->getChannel().empty())
        return "\033[1;31m\tYou have to be in a channel to send a message\033[0m\n";
    
    std::string message = "";
    for (size_t i = 0; i < request.args.size(); i++)
        message += request.args[i] + " ";
    message = message.substr(0, message.size() - 1);

    std::string response = "\033[1;33m\t" + clientList[fd]->getNickName() + ":\033[0m " + message + "\n";
    this->channels[clientList[fd]->getChannel()]->sendToAll(response);
    return "";
}