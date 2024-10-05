/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 03:54:43 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/19 15:40:49 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"

std::string IrcServ::privmsg(Request request, int fd) {

    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected to the server\033[0m\n";
    if (clientList[fd] && !clientList[fd]->getIsRegistered())
        return "\033[1;31m\tYou're not registered\033[0m\n";
    if (request.args.size() < 2)
        return "\033[1;31m\tUsage: /privmsg <nickname> <message>\033[0m\n";
    
    for (std::map<int, Client*>::iterator it = clientList.begin(); it != clientList.end(); it++) {
        if (it->second->getNickName() == request.args[0] && it->second->getIsRegistered()) {
            std::string msg = "\033[1;33m\t" + clientList[fd]->getNickName() + ":\033[0m ";
            for (size_t i = 1; i < request.args.size(); i++) {
                msg += request.args[i] + " ";
            }
            msg += "\n";
            send(it->first, msg.c_str(), msg.size(), 0);
            return "";
        }
    }

    return "\033[1;31m\tUser not found\033[0m\n";
}