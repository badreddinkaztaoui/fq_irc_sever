/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 19:45:37 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/22 17:52:39 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"

std::string    isVaildNick(std::string& nickname, std::vector<std::string>& nicknames) {
    std::transform(nickname.begin(), nickname.end(), nickname.begin(), ::tolower);

    if (nickname.size() > 9)
        return "\033[1;31m\tNickname is too long\033[0m\n";
    if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_") != std::string::npos)
        return "\033[1;31m\tNickname contains invalid characters\033[0m\n";
    for (size_t i = 0; i < nicknames.size(); i++) {
        if (nicknames[i] == nickname)
            return "\033[1;31m\tNickname is already in use\033[0m\n";
    }
    return "";
}

std::string handleRegistredNick(Client *client, std::vector<std::string>& nicknames, std::string& nickname) {
    if (client->getRegistrationStep() < 1)
        return "\033[1;31m\tYou have to set a password first\033[0m\n";
    else if (client->getRegistrationStep() >= 2)
        return "\033[1;31m\tYou've already completed this step\033[0m\n";
    else if (!isVaildNick(nickname, nicknames).empty())
        return isVaildNick(nickname, nicknames);
    else {
        client->setRegistrationStep(2);
        client->setNickName(nickname);
        nicknames.push_back(nickname);
        std::string msg = "\033[1;32m\tYour nickname has been set to:\033[0m " + client->getNickName() + "\n";
        send(client->getClientFd(), msg.c_str(), msg.size(), 0);
        return "\033[1;32m\tStep 2 Completed\033[0m\n";
    }
}

std::string handleUnregistredNick(Client *client, std::vector<std::string>& nicknames, std::string& nickname) {
    if (!isVaildNick(nickname, nicknames).empty())
        return isVaildNick(nickname, nicknames);
    else {
        client->setNickName(nickname);
        nicknames.push_back(nickname);
        return "\033[1;32m\tYour nickname has been set to:\033[0m " + nickname + "\n";
    }
}

std::string IrcServ::nick(Request request, int fd) {
    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected to the server\033[0m\n";
    if (request.args.size() != 1)
        return "\033[1;31m\tNo nickname provided\033[0m\n";
    if (!clientList[fd]->getIsRegistered())
        return handleRegistredNick(clientList[fd], this->nicknames, request.args[0]);
    else
        return handleUnregistredNick(clientList[fd], this->nicknames, request.args[0]);
}