/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boot.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 15:33:49 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/16 15:50:04 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Request.hpp"
#include "../../include/IrcServ.hpp"

void IrcServ::boot(Request request, int fd) {
    if (request.args.size() != 0)
        return send(fd, "\033[1;31m\tUsage: /HELP\033[0m\n", 29, 0), void();

    if (!clientList[fd]) {
        send(fd, "\033[1;31m\tYou're not connected\033[0m\n", 36, 0);
        return;
    }

    if (clientList[fd] && clientList[fd]->getIsRegistered() == false) {
        send(fd, "\033[1;31m\tYou have to be authenticated to use this command\033[0m\n", 61, 0);
        return;
    }

    std::string response = "\033[1;34m\tAvailable Commands:\033[0m\n\n";

    struct CommandInfo {
        const char* command;
        const char* description;
    };

    CommandInfo commands[] = {
        {"/HELP", "Displays this help message"},
        {"/NICK <nickname>", "Sets your nickname"},
        {"/USER <username> <hostname> <servername> <realname>", "Registers your user information"},
        {"/JOIN <#channel>", "Joins a channel"},
        {"/KICK <#channel> <nickname>", "Kicks a user from a channel"},
        {"/INVITE <nickname> <#channel>", "Invites a user to a channel"},
        {"/TOPIC <#channel> <topic>", "Sets the topic of a channel"},
        {"/MODE <#channel> <mode>", "Sets channel modes"},
        {"/PRIVMSG <nickname> <message>", "Sends a private message"},
        {"/MSG <#channel> <message>", "Sends a message to a channel"},
        {"/QUIT", "Disconnects from the server"}
    };

    const int numCommands = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < numCommands; ++i) {
        response += "\033[1;32m\t" + std::string(commands[i].command) + "\033[0m: " + commands[i].description + "\n";
    }

    send(fd, response.c_str(), response.length(), 0);
}