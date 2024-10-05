/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:31:53 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/17 02:39:44 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Channel.hpp"
#include "../../include/IrcServ.hpp"

std::string IrcServ::quit(Request request, int fd) {
    if (request.args.size() != 0)
        return "\033[1;31m\tUsage: /QUIT <reason>\033[0m\n";

    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected\033[0m\n";

    std::string response = "\t" + clientList[fd]->getNickName() + "\033[1;31m: Disconnected from the server\033[0m\n";
    send(fd, response.c_str(), response.length(), 0);
    
    if (clientList[fd]->getChannel() != "") {
        std::string channel = clientList[fd]->getChannel();
        channels[channel]->removeClient(clientList[fd]);
        if (channels[channel]->getConnectedClients() == 0) {
            delete channels[channel];
            channels.erase(channel);
        }
    }

    nicknames.erase(std::remove(nicknames.begin(), nicknames.end(), clientList[fd]->getNickName()), nicknames.end());
    usernames.erase(std::remove(usernames.begin(), usernames.end(), clientList[fd]->getUserName()), usernames.end());

    clientList[fd]->setNickName("Default");
    clientList[fd]->setUserName("Default");
    clientList[fd]->setRealName("Default");
    clientList[fd]->setHostname("localhost");
    clientList[fd]->setServername("irc.1337.ma");    
    
    for (std::map<int, Client*>::iterator it = clientList.begin(); it != clientList.end(); it++) {
        if (it->first == fd) {
            clientList.erase(it);
            break;
        }
    }
    return "";
}