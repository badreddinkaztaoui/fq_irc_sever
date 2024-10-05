/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 04:07:05 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/18 18:06:05 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_H
#define CHANNEL_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>

#include "Client.hpp"
#include "IrcServ.hpp"

#define MAX_BUFF 4096
#define MAX_EVENTS 20

class Client;

class Channel {
private:
    time_t                      creationTime;
    Client*                     admin;
    std::vector<int>            operators;
    std::map<int, Client*>      clients;
    std::vector<std::string>    invited;
    std::string                 name;
    std::string                 topic;
    std::string                 password;
    int                         limit;
    int                         connectedClients;
    bool                        isInviteOnly;
    bool                        hasPassword;
    bool                        isTopicProtected;
    bool                        isLimited;
public:
    Channel();
    Channel(std::string name);

    // Getters
    std::string                 getName();
    std::string                 getTopic();
    std::string                 getPassword();
    time_t                      getCreationTime();
    int                         getConnectedClients();
    bool                        getInviteOnly();
    bool                        getHasPassword();
    bool                        getTopicProtected();
    Client*                     getAdmin();
    bool                        getIsLimited();
    int                         getLimit();
    std::map<int, Client*>      getClients();

    // Setters
    void                        setAdmin(Client* client);
    void                        setTopic(std::string topic);
    void                        setPassword(std::string password);
    void                        setOperator(Client* client);
    void                        removeOperator(Client* client);
    void                        setLimit(int limit);
    void                        setIsLimited(bool isLimited);
    void                        setConnectedClients();
    void                        setInviteOnly(bool isInviteOnly);
    void                        setHasPassword(bool hasPassword);
    void                        setTopicProtected(bool isTopicProtected);
    void                        setInvited(std::string nickname);
    void                        removeInvited(std::string nickname);

    // Methods
    void                        addClient(Client* client);
    void                        removeClient(Client* client);
    void                        sendToAll(std::string msg);

    // Checkers
    bool                        isClient(Client* client);
    bool                        isOperator(Client* client);

    ~Channel();
};

#endif