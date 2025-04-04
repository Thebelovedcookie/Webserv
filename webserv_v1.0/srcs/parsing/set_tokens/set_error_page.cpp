/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_error_page.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:47 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/18 00:26:37 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_error_page_param(std::vector<std::string> tokens)
{
    std::stringstream ss(tokens[1]);
    int code = 0;
    ss >> code;

    std::string error_page = tokens[2];

    if (tokens.size() != 3 || tokens[1].empty() || tokens[2].empty() || this->location_started)
    {
        ft_invalid_line(tokens);
    }

	this->currentConfig->setErrorPages(code, error_page);
}
