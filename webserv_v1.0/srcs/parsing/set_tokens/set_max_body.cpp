/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_max_body.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:34 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/18 00:26:24 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_client_max_body_param(std::vector<std::string> tokens)
{
    if (tokens.size() != 2 || tokens[1].empty() || this->location_started)
    {
        ft_invalid_line(tokens);
    }

    this->currentConfig->setClientMaxBodySize(tokens[1]);
}
