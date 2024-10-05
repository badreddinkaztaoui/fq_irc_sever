/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 03:44:12 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/19 15:35:15 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"
#include "../../include/Utils.hpp"

std::string    isValidUsername(std::string username, std::vector<std::string> usernames) {
    std::transform(username.begin(), username.end(), username.begin(), ::tolower);

    if (username.size() > 9)
        return "\033[1;31m\tUsername is too long\033[0m\n";
    if (username.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_") != std::string::npos)
        return "\033[1;31m\tUsername contains invalid characters\033[0m\n";
    for (size_t i = 0; i < usernames.size(); i++) {
        if (usernames[i] == username)
            return "\033[1;31m\tUsername is already in use\033[0m\n";
    }
    return "";
}

std::string handleRegistredUser(Client *client, std::vector<std::string>& usernames, Request request) {
    if (client->getRegistrationStep() < 2)
        return "\033[1;31m\tYou have to set password and nickname first\033[0m\n";
    else if (!isValidUsername(request.args[0], usernames).empty())
        return isValidUsername(request.args[0], usernames);
    else {
        client->setUserName(request.args[0]);
        client->setRealName(request.args[3]);
        client->setHostname(request.args[1]);
        client->setServername(request.args[2]);
        usernames.push_back(request.args[0]);
        
        client->setRegistrationStep(3);
        client->setIsRegistered(true);
        
        std::string msg = "\033[1;32m\tYour username has been set to:\033[0m " + client->getNickName() + "\n";
        send(client->getClientFd(), msg.c_str(), msg.size(), 0);
        
        registredMsg(client->getClientFd());
        return "";
    }
}

std::string handleUnregistredUser(Client *client, std::vector<std::string>& usernames, Request request) {
    if (!isValidUsername(request.args[0], usernames).empty())
        return isValidUsername(request.args[0], usernames);
    else {
        client->setUserName(request.args[0]);
        client->setRealName(request.args[3]);
        client->setHostname(request.args[1]);
        client->setServername(request.args[2]);
        usernames.push_back(request.args[0]);
        return "\033[1;32m\tYour username has been set to:\033[0m " + request.args[0] + "\n";
    }
}

std::string IrcServ::user(Request request, int fd) {
    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected to the server\033[0m\n";
    if (request.args.size() != 4 || request.args[3][0] != ':')
        return "\033[1;33m\tUsage: /user <username> <hostname> <servername> :<realname>\033[0m\n";
    if (!clientList[fd]->getIsRegistered())
        return handleRegistredUser(clientList[fd], this->usernames, request);
    else
        return handleUnregistredUser(clientList[fd], this->usernames, request);
}