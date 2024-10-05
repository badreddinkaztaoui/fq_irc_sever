/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:37:47 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/18 18:08:54 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Utils.hpp"

std::string handleMode(Channel* channel, std::map<int, Client*>& clients, Request& request) {
    std::string response = "";

    if (request.args.size() < 2) {
        return "\033[1;31m\tInvalid mode command\033[0m\n";
    }

    char mode = request.args[1][1];
    bool set = request.args[1][0] == '+';
    Client* client = NULL;

    switch (mode) {
        case 'i':
            channel->setInviteOnly(set);
            if (set) {
                sendToChannel(channel->getName(), "\033[1;32m\tChannel is now invite only\033[0m\n", clients, 0);
            } else {
                sendToChannel(channel->getName(), "\033[1;33m\tChannel is no longer invite only\033[0m\n", clients, 0);
            }
            break;
        case 't':
            channel->setTopicProtected(set);
            if (set) {
                sendToChannel(channel->getName(), "\033[1;32m\tChannel topic is now protected\033[0m\n", clients, 0);
            } else {
                sendToChannel(channel->getName(), "\033[1;33m\tChannel topic is no longer protected\033[0m\n", clients, 0);
            }
            break;
        case 'k':
            channel->setHasPassword(set);
            if (set && request.args.size() < 3) {
                return "\033[1;31m\tUsage: /MODE <channel> +k <password>\033[0m\n";
            }
            if (set) {
                channel->setHasPassword(true);
                channel->setPassword(request.args[2]);
                sendToChannel(channel->getName(), "\033[1;32m\tChannel password has been set\033[0m\n", clients, 0);
            } else {
                channel->setHasPassword(false);
                sendToChannel(channel->getName(), "\033[1;33m\tChannel password has been removed\033[0m\n", clients, 0);
            }
            break;
        case 'o':
            if (request.args.size() < 3) {
                return "\033[1;31m\tUsage: /MODE <channel> +o <nickname>\033[0m\n";
            }
            if (!isClientInChannel(request.args[2], request.args[0], clients)) {
                return "\033[1;31m\tUser not found\033[0m\n";
            }

            client = findClient(request.args[2], clients);

            if (set) {
                if (channel->isOperator(client)) {
                    return "\033[1;31m\tUser is already an operator\033[0m\n";
                }
                channel->setOperator(client);
                client->setIsOperator(true);
                std::string msg = "\033[1;31m\tYou are now an operator in " + channel->getName() + "\033[0m\n";
                send(client->getClientFd(), msg.c_str(), msg.length(), 0);
            } else {
                if (!channel->isOperator(client)) {
                    return "\033[1;31m\tUser is not an operator\033[0m\n";
                }
                channel->removeOperator(client);
                client->setIsOperator(false);
                std::string msg = "\033[1;31m\tYou are no longer an operator in " + channel->getName() + "\033[0m\n";
                send(client->getClientFd(), msg.c_str(), msg.length(), 0);
            }
            break;
        case 'l':
            channel->setIsLimited(set);
            if (set && request.args.size() < 3) {
                return "\033[1;31m\tUsage: /MODE <channel> +l <limit>\033[0m\n";
            }
            if (set) {
                channel->setLimit(atoi(request.args[2].c_str()));
                sendToChannel(channel->getName(), "\033[1;31m\tChannel limit has been set\033[0m\n", clients, 0);
            }
            break;
        default:
            response = "\033[1;31m\tModes: +i, -i, +t, -t, +k, -k, +o, -o, +l, -l\033[0m\n";
            break;
    }

    return response;
}

std::string IrcServ::mode(Request request, int fd) {
    std::string response = "";
    
    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected to the server\033[0m\n";
    if (clientList[fd] && !clientList[fd]->getIsRegistered())
        return "\033[1;31m\tYou have to be authenticated to change mode\033[0m\n";
    if (request.args.size() < 2 || request.args.size() > 3)
        return "\033[1;31m\tUsage: /MODE <channel> <mode>\033[0m\n";

    Channel* channel = channels[request.args[0]];

    if (!channel)
        return "\033[1;31m\tChannel not found\033[0m\n";
    
    if (!channel->isOperator(clientList[fd]) && !isChannelAdmin(clientList[fd], channel))
        return "\033[1;31m\tYou're not an operator in this channel\033[0m\n";
    
    if (request.args[1].size() < 2 || (request.args[1][0] != '+' && request.args[1][0] != '-'))
        return "\033[1;31m\tInvalid mode\033[0m\n";
    
    response = handleMode(channel, clientList, request);

    return response;
}