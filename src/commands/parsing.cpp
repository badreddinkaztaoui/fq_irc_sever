/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:21:52 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/22 17:58:48 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"
#include "../../include/Client.hpp"


Request IrcServ::parseResponse(std::string response) {
    Request request;
    request.status = 0;

    std::cout << "Response: " << response << std::endl;
    
    if (response.empty() || response == "\n")
        request.status = 1;

    std::istringstream iss(response);
    std::vector<std::string> args;

    std::string cmd;
    iss >> cmd; 

    if (cmd[0] != '/')
        request.status = 1;

    for (std::string arg; iss >> arg;)
        args.push_back(arg);

    request.cmd = cmd;
    request.args = args;
    return request;
}

std::string IrcServ::parseRequest(Request request, int fd) {
    std::string response;

    if (request.status == 1)
        return "\033[1;31m\tInvalid command\033[0m\n";
    
    request.cmd = request.cmd.substr(1);
    std::transform(request.cmd.begin(), request.cmd.end(), request.cmd.begin(), ::tolower);
    
    if (request.cmd == "connect")
        response = connect(request, fd);
    else if (request.cmd == "help")
        boot(request, fd);
    else if (request.cmd == "pass")
        response = pass(request, fd);
    else if (request.cmd == "nick")
        response = nick(request, fd);
    else if (request.cmd == "user")
        response = user(request, fd);
    else if (request.cmd == "msg")
        response = msg(request, fd);
    else if (request.cmd == "privmsg")
        response = privmsg(request, fd);
    else if (request.cmd == "join")
        response = join(request, fd);
    else if (request.cmd == "invite")
        response = invite(request, fd);
    else if (request.cmd == "topic")
        response = topic(request, fd);
    else if (request.cmd == "kick")
        response = kick(request, fd);
    else if (request.cmd == "mode")
        response = mode(request, fd);
    else if (request.cmd == "quit")
        response = quit(request, fd);
    else
        response = "\033[1;31m\tInvalid command\033[0m\n";
    return response;
}
