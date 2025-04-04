/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_location.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:38 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/28 16:57:40 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_location_param(std::vector<std::string> tokens)
{
    this->location_started = true;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].empty())
        {
            ft_invalid_line(tokens);
        }
    }

    if (tokens[1][0] == '/' || (tokens[1] == "=" && tokens[2][0] == '/'))
    {
        ft_location_pages(tokens);
    }
    else
    {
        ft_invalid_line(tokens);
    }
}
