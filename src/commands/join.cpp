/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:24:59 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/18 15:36:33 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Request.hpp"
#include "../../include/IrcServ.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Utils.hpp"

void    IrcServ::broadcast(std::string channel, int fd) {

    time_t date = this->channels[channel]->getCreationTime();
    std::string message = "\n";

    message += "\033[1;32m\t" + this->clientList[fd]->getNickName() + " has joined \033[0m" + channel + "\n";
    message += "\033[1;32m\tWelcome to the channel \033[0m" + channel + "\n";
    message += "\033[1;32m\tTopic: \033[0m" + this->channels[channel]->getTopic() + "\n";
    message += "\033[1;32m\tChannel created on: \033[0m" + std::string(ctime(&date));
    message += "\033[1;32m\tCreator: \033[0m" + this->channels[channel]->getAdmin()->getNickName() + "\n";

    std::string notif = "\033[1;33m\n\t" + this->clientList[fd]->getNickName() + " has joined the channel\033[0m\n";

    send(fd, message.c_str(), message.size(), 0);
    sendToChannel(channel, notif, this->clientList, fd);
}

std::string handleChannelPassword(std::map<std::string, Channel*>&channels, std::string channel, std::string pass) {
    if (channels[channel]->getPassword() != pass)
        return "\033[1;31m\tInvalid password\033[0m\n";
    
    return "";
}

std::string IrcServ::join(Request request, int fd) {
    std::string response = "";

    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected to the server\033[0m\n";
    if (clientList[fd] && clientList[fd]->getIsRegistered() == false)
        return "\033[1;31m\tYou have to be authenticated to join a channel\033[0m\n";
    if (request.args.size() < 1 || request.args.size() > 2)
        return "\033[1;31m\tUsage: /JOIN #channel\033[0m\n";
    
    std::string channel = request.args[0].substr(0, request.args[0].find('\n'));

    if (!isValidChannelName(channel))
        return "\033[1;31m\tInvalid channel name\033[0m\n";
    if (clientList[fd]->getChannel() == channel)
        return "\033[1;31m\tYou're already in this channel\033[0m\n";
    
    if (!findChannel(channel, this->channels))
        createChannel(channel, fd);
    else {
        if (this->channels[channel]->getHasPassword()) {
            if (request.args.size() < 2)
                return "\033[1;31m\tChannel is password protected, use /join #channel <password>\033[0m\n";
            response = handleChannelPassword(this->channels, channel, request.args[1]);
        }
        if (this->channels[channel]->getIsLimited() && this->channels[channel]->getConnectedClients() >= this->channels[channel]->getLimit())
            return "\033[1;31m\tChannel is full\033[0m\n";
        if (this->channels[channel]->getInviteOnly()) {
            if (!clientList[fd]->getIsInvited())
                return "\033[1;31m\tYou're not invited to this channel\033[0m\n";
        }

        if (clientList[fd]->getChannel() != "") {
            if (this->channels[clientList[fd]->getChannel()]->getConnectedClients() == 1) {
                this->channels.erase(clientList[fd]->getChannel());
            } else {
                this->channels[clientList[fd]->getChannel()]->removeClient(clientList[fd]);
                this->channels[clientList[fd]->getChannel()]->setConnectedClients();
            }
        }

        this->clientList[fd]->setChannel(channel);
        this->channels[channel]->addClient(clientList[fd]);
        this->channels[channel]->setConnectedClients();
        broadcast(channel, fd);
    }    
    return response;
}