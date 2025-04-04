/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_allowed_methods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:38:20 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/24 12:54:03 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_set_allowed_methods(std::vector<std::string> tokens)
{
	this->currentConfig->setAllowedMethods("GET", 0);
	this->currentConfig->setAllowedMethods("POST", 0);
	this->currentConfig->setAllowedMethods("DELETE", 0);

	for (size_t i = 1; i < tokens.size(); i++)
	{
		if (tokens[i].empty() || (tokens[i] != "GET" && tokens[i] != "POST" && tokens[i] != "DELETE") || this->location_started)
		{
			ft_invalid_line(tokens);
		}
		else
		{
			if (tokens[i] == "GET")
			{
				this->currentConfig->setAllowedMethods("GET", 1);
			}
			else if (tokens[i] == "POST")
			{
				this->currentConfig->setAllowedMethods("POST", 1);
			}
			else if (tokens[i] == "DELETE")
			{
				this->currentConfig->setAllowedMethods("DELETE", 1);
			}
		}
	}
}
