/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaztaou <bkaztaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:00:27 by bkaztaou          #+#    #+#             */
/*   Updated: 2024/08/14 10:47:04 by bkaztaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/IrcServ.hpp"
#include "../../include/Request.hpp"

std::string IrcServ::pass(Request request, int fd) {
    if (!clientList[fd])
        return "\033[1;31m\tYou're not connected to the server\033[0m\n";
    if (clientList[fd] && clientList[fd]->getIsRegistered() == 1)
        return "\033[1;31m\tYou're already registered\033[0m\n";
    if (request.args.size() != 1)
        return "\033[1;31m\tUsage: /PASS <password>\033[0m\n";
    if (request.args[0] != this->passWord_)
        return "\033[1;31m\tInvalid password\033[0m\n";
    if (!clientList[fd]->getIsRegistered()) {
        if (clientList[fd]->getRegistrationStep() >= 1)
            return "\033[1;31m\tYou've already completed this step\033[0m\n";
        this->clientList[fd]->setRegistrationStep(1);
        return "\033[1;32m\tStep 1 Completed\033[0m\n";
    }
    return "\033[1;31m\tInvalid command\033[0m\n";
}