/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:53:45 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/18 18:03:59 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Channel.hpp"
#include "../../include/IrcServ.hpp"
#include "../../include/Utils.hpp"

std::string IrcServ::topic(Request request, int fd) {
    if (request.args.size() < 1 || request.args[0][0] != '#' || request.args.size() > 2)
        return "\033[1;31m\tUsage: /TOPIC #channel +<topic>\033[0m\n";
    if (!clientList[fd] || !clientList[fd]->getIsRegistered())
        return "\033[1;31m\tYou have to be authenticated to set a topic\033[0m\n";
    if (clientList[fd]->getChannel() == "")
        return "\033[1;31m\tYou have to join a channel first\033[0m\n";

    Channel* channel = findChannel(request.args[0], this->channels);
    if (!channel)
        return "\033[1;31m\tChannel not found\033[0m\n";
        
    std::string nickname = clientList[fd]->getNickName();
    if (request.args.size() == 1) {
        if (isClientInChannel(nickname, channel->getName(), clientList))
            return "\033[1;33m\tTopic of the channel is: \033[0m" + channel->getTopic() + "\n";
        return "\033[1;31m\tYou're not in this channel\033[0m\n";
    }

    if (channel->getTopicProtected() && !isChannelAdmin(clientList[fd], channel))
        return "\033[1;31m\tTopic is protected\033[0m\n";

    if (channel->isOperator(clientList[fd]) || isChannelAdmin(clientList[fd], channel)) {
        channel->setTopic(request.args[1]);
        return "\033[1;32m\tTopic set successfully to: \033[0m" + request.args[1] + "\n";
    } else {
        return "\033[1;31m\tYou have to be an operator to set the topic\033[0m\n";
    }
}