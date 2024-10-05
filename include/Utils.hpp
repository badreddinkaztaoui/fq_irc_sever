/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 04:17:14 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/18 16:57:34 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <map>
#include "Channel.hpp"
#include "Client.hpp"

std::string toStr(int num);
Channel*    findChannel(const std::string& name, std::map<std::string, Channel*>& channels);
Client*     findClient(const std::string& name, std::map<int, Client*>& clientList);
bool        isClientInChannel(std::string& nickname, std::string channel, std::map<int ,Client*>& clients);
bool        isValidChannelName(const std::string& name);
bool        isChannelAdmin(Client* client, Channel* channel);
void        connectedMsg(int fd);
void        registredMsg(int fd);
void        sendToChannel(std::string channel, std::string msg, std::map<int, Client*>& clients, int exclude);


#endif