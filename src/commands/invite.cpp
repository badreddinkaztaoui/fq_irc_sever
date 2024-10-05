/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:54:52 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/18 14:55:43 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Utils.hpp"

std::string IrcServ::invite(Request request, int fd) {
    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected to the server\033[0m\n";
    if (request.args.size() != 2)
        return "\033[1;31m\tUsage: /INVITE <nickname> <channel>\033[0m\n";
    if (clientList[fd]->getChannel().empty())
        return "\033[1;31m\tYou have to be in a channel to invite someone\033[0m\n";
    
    Channel* channel = findChannel(request.args[1], this->channels);
    if (!channel)
        return "\033[1;31m\tChannel not found\033[0m\n";

    if (!channel->isOperator(clientList[fd]) && !isChannelAdmin(clientList[fd], channel))
        return "\033[1;31m\tYou're not an operator in this channel\033[0m\n";

    Client* client = findClient(request.args[0], clientList);

    if (!client)
        return "\033[1;31m\tClient not found\033[0m\n";

    if (client->getChannel() == channel->getName())
        return "\033[1;31m\tClient is already in the channel\033[0m\n";
    
    client->setIsInvited(true);

    std::string msg = "\033[1;32m\tYou have been invited to " + channel->getName() + "\033[0m\n";
    send(client->getClientFd(), msg.c_str(), msg.length(), 0);
    return "\033[1;32m\tInvite sent\033[0m\n";
}