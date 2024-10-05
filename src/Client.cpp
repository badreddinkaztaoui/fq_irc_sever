/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:59:01 by nben-ais          #+#    #+#             */
/*   Updated: 2024/08/18 14:57:35 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(): clientFd(-1), nickName("Guest"), userName("Guest"), realName("Guest"), hostName("localhost"), serverName("irc.1337.ma"), id(""), channel(""), registrationStep(0), isRegistered(false), isOperator(false), isInvited(false) {}

Client::Client(int fd): clientFd(fd), nickName("Guest"), userName("Guest"), realName("Guest"), hostName("localhost"), serverName("irc.1337.ma"), id(""), channel(""), registrationStep(0), isRegistered(false), isOperator(false), isInvited(false) {}

Client::Client(Client const &org) {
    *this = org;
}

Client &Client::operator=(Client const &org) {
    if (this != &org) {
        clientFd = org.clientFd;
        isRegistered = org.isRegistered;
        nickName = org.nickName;
        userName = org.userName;
        realName = org.realName;
        hostName = org.hostName;
        serverName = org.serverName;
        registrationStep = org.registrationStep;
        isOperator = org.isOperator;
        isInvited = org.isInvited;
        id = org.id;
        channel = org.channel;
    }
    return *this;
}

ssize_t Client::getRegistrationStep() {
    return this->registrationStep;
}

int Client::getClientFd() {
    return clientFd;
}

bool    Client::getIsRegistered() {
    return isRegistered;
}

std::string Client::getNickName() {
    return nickName;
}

std::string Client::getUserName() {
    return userName;
}

std::string Client::getRealName() {
    return realName;
}

std::string Client::getHostname() {
    return hostName;
}

std::string Client::getServername() {
    return serverName;
}

std::string Client::getChannel() {
    return channel;
}

bool    Client::getIsOperator() {
    return isOperator;
}

bool    Client::getIsInvited() {
    return isInvited;
}

// Setters

void    Client::setIsRegistered(bool reg) {
    isRegistered = reg;
}

void    Client::setRegistrationStep(ssize_t step) {
    this->registrationStep = step;
}

void    Client::setNickName(std::string nickname) {
    nickName = nickname;
}
void    Client::setUserName(std::string username) {
    userName = username;
}

void    Client::setRealName(std::string realname) {
    realName = realname;
}

void    Client::setHostname(std::string hostname) {
    hostName = hostname;
}

void    Client::setServername(std::string servername) {
    serverName = servername;
}

void    Client::setChannel(std::string channelName) {
    channel = channelName;
}

void    Client::setIsOperator(bool op) {
    isOperator = op;
}

void    Client::setIsInvited(bool inv) {
    isInvited = inv;
}

Client::~Client() {}
