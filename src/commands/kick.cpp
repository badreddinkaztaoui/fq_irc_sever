/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:13:03 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/18 18:16:07 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Utils.hpp"

std::string IrcServ::kick(Request request, int fd) {
    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected to the server\033[0m\n";
    if (request.args.size() != 3 || request.args[0][0] != '#')
        return "\033[1;31m\tUsage: /KICK #<channel> <nickname> <reason>\033[0m\n";
    if (clientList[fd]->getChannel().empty())
        return "\033[1;31m\tYou have to be in a channel to kick someone\033[0m\n";
    
    Channel* channel = findChannel(request.args[0], this->channels);
    
    if (!channel)
        return "\033[1;31m\tChannel not found\033[0m\n";
    
    if (!channel->isOperator(clientList[fd]) && !isChannelAdmin(clientList[fd], channel))
        return "\033[1;31m\tYou're not an operator in this channel\033[0m\n";
    
    if (!isClientInChannel(request.args[1], channel->getName(), clientList))
        return "\033[1;31m\tClient is not in this channel\033[0m\n";
    
    Client* client = findClient(request.args[1], clientList);
    
    if (!client)
        return "\033[1;31m\tClient not found\033[0m\n";

    if (channel->getAdmin()->getNickName() == client->getNickName()) {
        return "\033[1;31m\tYou can't kick the admin of the channel\033[0m\n";
    }

    client->setChannel("");
    channel->removeClient(client);

    std::string pubMsg = "\033[1;31m\t" + client->getNickName() + " has been kicked from " + channel->getName() + " because: " + request.args[2] + "\033[0m\n";
    std::string privMsg = "\033[1;31m\tYou have been kicked from " + channel->getName() + "\033[0m\n";
    
    channel->sendToAll(pubMsg);
    send(client->getClientFd(), privMsg.c_str(), privMsg.length(), 0);
    
    return "";
}