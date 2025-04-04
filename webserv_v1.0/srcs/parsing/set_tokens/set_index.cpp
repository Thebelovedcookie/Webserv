/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_index.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:07:06 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/18 00:26:31 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_index_param(std::vector<std::string> tokens)
{
    if (tokens.size() != 2 || tokens[1].empty() || this->location_started)
    {
        ft_invalid_line(tokens);
    }

    this->currentConfig->setIndex(tokens[1]);
}
