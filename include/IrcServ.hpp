/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:41:53 by nben-ais          #+#    #+#             */
/*   Updated: 2024/08/18 16:32:10 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_H
#define IRCSERV_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sstream>

// Classes
#include "Client.hpp"
#include "Request.hpp"
#include "Channel.hpp"

class Client;
class Channel;
class IrcServ {
    private:
        std::string                     port_;
        std::string                     passWord_;
        int                             serverSocket_;
        int                             epollFd_;
        std::map<int, Client*>          clientList;
        std::vector<std::string>        usernames;
        std::vector<std::string>        nicknames;
        std::map<std::string, Channel*> channels;

    public:
        IrcServ();
        IrcServ(const std::string& port, const std::string& password);

        // --------------------------------- //
        //--------- SERVER FUNCTIONS --------//
        // --------------------------------- //
        void                            createSocket();
        void                            setupEpoll();
        void                            acceptClient();
        void                            eventLoop();
        void                            handleClientMsg(int clientFd);
        void                            handleClientDisconnection(int clientFd);
        void                            run();
        // --------------------------------- //
        // ------------- PARSING ------------//
        // --------------------------------- //
        Request                         parseResponse(std::string response);
        std::string                     parseRequest(Request request, int fd);

        // --------------------------------- //
        // ------------ COMMANDS ------------//
        // --------------------------------- //
        std::string                     connect(Request request, int fd);
        std::string                     pass(Request request, int fd);
        std::string                     nick(Request request, int fd);
        std::string                     user(Request request, int fd);
        std::string                     privmsg(Request request, int fd);
        std::string                     msg(Request request, int fd);
        std::string                     join(Request request, int fd);
        std::string                     mode(Request request, int fd);
        std::string                     topic(Request request, int fd);
        std::string                     invite(Request request, int fd);
        std::string                     kick(Request request, int fd);
        std::string                     quit(Request request, int fd);
        void                            boot(Request request, int fd);
        // --------------------------------- //
        // ------------- CHANNELS -----------//
        // --------------------------------- //
        void                            createChannel(std::string channel, int fd);
        void                            broadcast(std::string channel, int fd);

        ~IrcServ();
};

void    errorFunction(const char *error, int serverSocket);

#endif 