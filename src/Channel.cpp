/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 04:47:39 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/19 15:44:24 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"
#include "../include/IrcServ.hpp"

Channel::Channel() {}

Channel::Channel(std::string name): name(name) {
    this->creationTime = time(0);
    this->connectedClients = 0;
    this->isInviteOnly = false;
    this->hasPassword = false;
    this->isTopicProtected = false;
    this->isLimited = false;
    this->password = "";
    this->limit = 0;
    this->topic = "Default topic";
}

void    IrcServ::createChannel(std::string name, int fd) {
    this->channels[name] = new Channel(name);
    this->clientList[fd]->setChannel(name);
    this->clientList[fd]->setIsOperator(true);
    this->channels[name]->addClient(this->clientList[fd]);
    this->channels[name]->setAdmin(this->clientList[fd]);
    this->channels[name]->setOperator(this->clientList[fd]);
    this->channels[name]->setConnectedClients();
    broadcast(name, fd);
}

// Getters
std::string Channel::getName() {
    return this->name;
}

std::string Channel::getTopic() {
    return this->topic;
}

std::string Channel::getPassword() {
    return this->password;
}

int     Channel::getConnectedClients() {
    return this->connectedClients;
}

time_t  Channel::getCreationTime() {
    return this->creationTime;
}

int     Channel::getLimit() {
    return this->limit;
}

bool    Channel::getInviteOnly() {
    return this->isInviteOnly;
}

bool    Channel::getHasPassword() {
    return this->hasPassword;
}

bool    Channel::getTopicProtected() {
    return this->isTopicProtected;
}

bool    Channel::getIsLimited() {
    return this->isLimited;
}

Client* Channel::getAdmin() {
    return this->admin;
}

std::map<int, Client*> Channel::getClients() {
    return this->clients;
}

// Setters
void    Channel::setTopic(std::string topic) {
    this->topic = topic;
}

void    Channel::setLimit(int limit) {
    this->limit = limit;
}

void    Channel::setIsLimited(bool isLimited) {
    this->isLimited = isLimited;
}

void    Channel::setConnectedClients() {
    this->connectedClients = this->clients.size();
}

void    Channel::setPassword(std::string password) {
    this->password = password;
}

void    Channel::setOperator(Client* client) {
    this->operators.push_back(client->getClientFd());
}

void    Channel::removeOperator(Client* client) {
    this->operators.erase(std::remove(this->operators.begin(), this->operators.end(), client->getClientFd()), this->operators.end());
}

void    Channel::setInviteOnly(bool isInviteOnly) {
    this->isInviteOnly = isInviteOnly;
}

void    Channel::setHasPassword(bool hasPassword) {
    this->hasPassword = hasPassword;
}

void    Channel::setTopicProtected(bool isTopicProtected) {
    this->isTopicProtected = isTopicProtected;
}

void    Channel::setAdmin(Client* client) {
    this->admin = client;
}

void    Channel::setInvited(std::string nickname) {
    this->invited.push_back(nickname);
}

void    Channel::removeInvited(std::string nickname) {
    this->invited.erase(std::remove(this->invited.begin(), this->invited.end(), nickname), this->invited.end());
}

// Methods
void Channel::addClient(Client* client) {
    this->clients[client->getClientFd()] = client;
}

void Channel::removeClient(Client* client) {
    this->clients.erase(client->getClientFd());
}

void Channel::sendToAll(std::string msg) {
    for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++) {
        send(it->second->getClientFd(), msg.c_str(), msg.length(), 0);
    }
}

bool Channel::isClient(Client* client) {
    return this->clients.find(client->getClientFd()) != this->clients.end();
}

bool Channel::isOperator(Client* client) {
    return std::find(this->operators.begin(), this->operators.end(), client->getClientFd()) != this->operators.end();
}


Channel::~Channel() {}

