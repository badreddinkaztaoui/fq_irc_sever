/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:41:23 by nben-ais          #+#    #+#             */
/*   Updated: 2024/08/18 16:41:27 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/IrcServ.hpp"
#include "../include/Request.hpp"
#include "../include/Utils.hpp"

IrcServ::IrcServ(): clientList() {}

IrcServ::IrcServ(const std::string& port, const std::string& password)
    : port_(port), passWord_(password), serverSocket_(-1), epollFd_(-1) {}

void    IrcServ::run() {
    createSocket();
    setupEpoll();
    eventLoop();
}

void IrcServ::createSocket() {
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        exit(1);
    }

    int optval = 1;
    if (setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        std::cerr << "Failed to set socket options: " << strerror(errno) << std::endl;
        close(serverSocket_);
        exit(1);
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(atoi(port_.c_str()));

    if (bind(serverSocket_, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;
        close(serverSocket_);
        exit(1);
    }

    if (listen(serverSocket_, SOMAXCONN) < 0) {
        std::cerr << "Failed to listen on socket: " << strerror(errno) << std::endl;
        close(serverSocket_);
        exit(1);
    }

    int flags = fcntl(serverSocket_, F_GETFL, 0);
    fcntl(serverSocket_, F_SETFL, flags | O_NONBLOCK);
}

void IrcServ::setupEpoll() {
    epollFd_ = epoll_create(1);
    if (epollFd_ < 0) {
        std::cerr << "Failed to create epoll instance: " << strerror(errno) << std::endl;
        close(serverSocket_);
        exit(1);
    }

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serverSocket_;
    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, serverSocket_, &event) < 0) {
        std::cerr << "Failed to add server socket to epoll: " << strerror(errno) << std::endl;
        close(serverSocket_);
        close(epollFd_);
        exit(1);
    }
}

void IrcServ::eventLoop() {
    epoll_event events[MAX_EVENTS];

    while (true) {
        int nfds = epoll_wait(epollFd_, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            std::cerr << "epoll_wait failed: " << strerror(errno) << std::endl;
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == serverSocket_) {
                acceptClient();
            } else {
                handleClientMsg(events[i].data.fd);
            }
        }
    }
}

void IrcServ::acceptClient() {
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientFd = accept(serverSocket_, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
    if (clientFd < 0) {
        std::cerr << "Failed to accept client connection: " << strerror(errno) << std::endl;
        return;
    }

    // Set non-blocking
    int flags = fcntl(clientFd, F_GETFL, 0);
    fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);

    epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = clientFd;
    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, clientFd, &event) < 0) {
        std::cerr << "Failed to add client to epoll: " << strerror(errno) << std::endl;
        close(clientFd);
        return;
    }

    clientList[clientFd] = new Client(clientFd);
    std::cout << "New client connected: " << clientFd << std::endl;
    connectedMsg(clientFd);
}

void IrcServ::handleClientMsg(int clientFd) {
    static std::map<int, std::string> clientBuffers;
    char buffer[MAX_BUFF];
    ssize_t bytesRead = 0;

    while ((bytesRead = recv(clientFd, buffer, MAX_BUFF - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        clientBuffers[clientFd].append(buffer);

        size_t pos;
        while ((pos = clientBuffers[clientFd].find('\n')) != std::string::npos) {
            std::string line = clientBuffers[clientFd].substr(0, pos);
            clientBuffers[clientFd].erase(0, pos + 1);

            line.erase(std::remove(line.begin(), line.end(), '\0'), line.end());

            if (!line.empty()) {
                Request request = parseResponse(line);
                std::string response = parseRequest(request, clientFd);
                send(clientFd, response.c_str(), response.size(), 0);
            }
        }

        if (clientBuffers[clientFd].size() >= MAX_BUFF) {
            send(clientFd, "Message too long\r\n", 24, 0);
            clientBuffers[clientFd].clear();
        }
    }

    if (bytesRead == 0) {
        std::cout << "Client disconnected: " << clientFd << std::endl;
        handleClientDisconnection(clientFd);
        clientBuffers.erase(clientFd);
        return;
    } else if (bytesRead < 0 && errno != EAGAIN) {
        std::cerr << "Failed to receive message from client: " << strerror(errno) << std::endl;
        handleClientDisconnection(clientFd);
        clientBuffers.erase(clientFd);
        return;
    }
}

void    IrcServ::handleClientDisconnection(int clientFd) {
    std::map<int, Client*>::iterator it = clientList.find(clientFd);
    if (it != clientList.end()) {
        Client* client = it->second;
        if (!client->getChannel().empty()) {
            std::map<std::string, Channel*>::iterator channelIt = channels.find(client->getChannel());
            if (channelIt != channels.end()) {
                Channel* channel = channelIt->second;
                channel->removeOperator(client);
                channel->removeClient(client);
                if (channel->getAdmin() == client) {
                    const std::map<int, Client*>& channelClients = channel->getClients();
                    if (!channelClients.empty()) {
                        channel->setAdmin(channelClients.begin()->second);
                    } else {
                        delete channel;
                        channels.erase(channelIt);
                    }
                }
            }
        }

        std::vector<std::string>::iterator nickIt = std::find(nicknames.begin(), nicknames.end(), client->getNickName());
        if (nickIt != nicknames.end()) {
            nicknames.erase(nickIt);
        }
        std::vector<std::string>::iterator userIt = std::find(usernames.begin(), usernames.end(), client->getUserName());
        if (userIt != usernames.end()) {
            usernames.erase(userIt);
        }
        delete client;
        clientList.erase(it);
    }
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, clientFd, NULL);
    close(clientFd);
}

IrcServ::~IrcServ() {
    std::map<int, Client*>::iterator it;
    for (it = clientList.begin(); it != clientList.end(); ++it) {
        delete it->second;
    }
    std::map<std::string, Channel*>::iterator chIt;
    for (chIt = channels.begin(); chIt != channels.end(); ++chIt) {
        delete chIt->second;
    }
    if (serverSocket_ != -1) close(serverSocket_);
    if (epollFd_ != -1) close(epollFd_);
}
