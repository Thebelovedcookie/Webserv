/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_server_name.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:31 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/24 12:47:08 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_server_name_param(std::vector<std::string> tokens)
{
    if (tokens.size() < 2 || tokens[1].empty() || this->location_started)
    {
        ft_invalid_line(tokens);
    }

    for (size_t i = 1; i < tokens.size(); i++)
    {
        this->currentConfig->setServerName(tokens[i]);
    }
}