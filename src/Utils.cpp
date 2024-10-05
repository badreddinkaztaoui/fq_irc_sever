/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 04:16:19 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/18 16:57:55 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Utils.hpp"

void connectedMsg(int fd) {
    std::string msg = "";

    msg += "\033[1;32m\t██████╗  █████╗ ███╗   ██╗        ██╗██████╗  ██████╗\033[0m\n";
    msg += "\033[1;32m\t██╔══██╗██╔══██╗████╗  ██║        ██║██╔══██╗██╔════╝\033[0m\n";
    msg += "\033[1;32m\t██████╔╝███████║██╔██╗ ██║        ██║██████╔╝██║     \033[0m\n";
    msg += "\033[1;32m\t██╔══██╗██╔══██║██║╚██╗██║        ██║██╔══██╗██║     \033[0m\n";
    msg += "\033[1;32m\t██████╔╝██║  ██║██║ ╚████║███████╗██║██║  ██║╚██████╗\033[0m\n";
    msg += "\033[1;32m\t╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝╚═╝╚═╝  ╚═╝ ╚═════╝\033[0m\n\n";

    msg += "\033[1m\tSTEP 1: Verify server password 🗝️\033[0m\n";
    msg += "\033[1;32m\t/PASS\033[0m <password>\n";
    msg += "\033[1m\tSTEP 2: Set your nickname 👤\033[0m\n";
    msg += "\033[1;32m\t/NICK\033[0m <nickname>\n";
    msg += "\033[1m\tSTEP 3: Set your username 📝\033[0m\n";
    msg += "\033[1;32m\t/USER\033[0m <username> <hostname> <servername> :<realname>\n\n";

    send(fd, msg.c_str(), msg.length(), 0);
}

void    registredMsg(int fd) {
    std::string msg = "";
    msg += "\033[1;33m\t┓ ┏  ┓                                  \033[0m\n";
    msg += "\033[1;33m\t┃┃┃┏┓┃┏┏┓┏┳┓┏┓  ╋┏┓  ┏┓┓┏┏┓  ┏┏┓┏┓┓┏┏┓┏┓\033[0m\n";
    msg += "\033[1;33m\t┗┻┛┗ ┗┗┗┛┛┗┗┗   ┗┗┛  ┗┛┗┻┛   ┛┗ ┛ ┗┛┗ ┛ \033[0m\n";
    msg += "\033[1;34m\tUse /HELP to see available commands\033[0m\n";
    send(fd, msg.c_str(), msg.length(), 0);
}

std::string toStr(int num) {
    std::stringstream str;
    str << num;
    return str.str();
}

Channel* findChannel(const std::string& name, std::map<std::string, Channel*>& channels) {
    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it++) {
        if (it->first == name) {
            return it->second;
        }
    }
    return NULL; 
}

Client *findClient(const std::string& name, std::map<int, Client*>& clientList) {
    for (std::map<int, Client*>::iterator it = clientList.begin(); it != clientList.end(); it++) {
        if (it->second->getNickName() == name) {
            return it->second;
        }
    }
    return NULL;
}

bool    isClientInChannel(std::string& nickname, std::string channel, std::map<int ,Client*>& clients) {
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
        if (it->second->getNickName() == nickname && it->second->getChannel() == channel) {
            return true;
        }
    }
    return false;
}

bool    isValidChannelName(const std::string& name) {
    if (name[0] != '#')
        return false;
    return true;
}

void    sendToChannel(std::string channel, std::string msg, std::map<int, Client*>& clients, int exclude) {
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
        if (it->second->getChannel() == channel) {
            if (exclude &&  it->first == exclude)
                continue;
            send(it->first, msg.c_str(), msg.length(), 0);
        }
    }
}

bool    isChannelAdmin(Client* client, Channel* channel) {
    return client == channel->getAdmin();
}