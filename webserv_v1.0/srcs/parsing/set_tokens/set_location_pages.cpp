/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_location_pages.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 15:15:09 by sguillot          #+#    #+#             */
/*   Updated: 2024/10/01 13:40:20 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_location_pages_dispatch(std::vector<std::string> current_param, Location &new_location)
{
	std::vector<std::string>::iterator current_param_it = current_param.begin();

	if (*current_param_it == "alias" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.setAlias(current_param[1]);
	}
	else if (*current_param_it == "root" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.setRoot(current_param[1]);
	}
	else if (*current_param_it == "client_max_body_size" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.setClientMaxBodySize(atoi(current_param[1].c_str()));
	}
	else if (*current_param_it == "auto_index" && current_param.size() == 2 && !current_param[1].empty())
	{
		if (current_param[1] == "on")
			new_location.setAutoIndex(1);
		else if (current_param[1] == "off")
			new_location.setAutoIndex(0);
		else
			ft_invalid_line(current_param);
	}
	else if (*current_param_it == "index" && current_param.size() == 2 && !current_param[1].empty())
	{
		new_location.setIndex(current_param[1]);
	}
	else if (*current_param_it == "allowed_methods" && current_param.size() >= 2)
	{
		new_location.setAllowedMethods("GET", 0);
		new_location.setAllowedMethods("POST", 0);
		new_location.setAllowedMethods("DELETE", 0);
		for (size_t i = 1; i < current_param.size(); i++)
		{
			if (current_param[i].empty() || (current_param[i] != "GET" && current_param[i] != "POST" && current_param[i] != "DELETE"))
			{
				ft_invalid_line(current_param);
			}
			else
			{
				if (current_param[i] == "GET")
				{
					new_location.setAllowedMethods("GET", 1);
				}
				else if (current_param[i] == "POST")
				{
					new_location.setAllowedMethods("POST", 1);
				}
				else if (current_param[i] == "DELETE")
				{
					new_location.setAllowedMethods("DELETE", 1);
				}
			}
		}
	}
	else if (*current_param_it == "error_page" && current_param.size() == 3 && !current_param[1].empty() && !current_param[2].empty())
	{
		new_location.setErrorPage(atoi(current_param[1].c_str()), current_param[2]);
	}
	else if (*current_param_it == "return" && current_param.size() == 3 && !current_param[1].empty() && !current_param[2].empty())
	{
		new_location.setRedirection(atoi(current_param[1].c_str()), current_param[2]);
	}
	else if (*current_param_it == "cgi" && (current_param.size() % 2 == 1) && current_param.size() >= 3 && !current_param[1].empty() && !current_param[2].empty())
	{
		for (size_t i = 0; (i + 2) < current_param.size(); i += 2)
		{
			new_location.setCgi(current_param[1 + i], current_param[2 + i]);
		}
	}
	else
	{
		ft_invalid_line(current_param);
	}
}

void Server::ft_location_pages(std::vector<std::string> tokens)
{
	std::vector<std::string> current_param;
	Location new_location;

	if (tokens.size() < 5 || (tokens[2] != "|" && tokens[1][0] == '/') || (tokens[3] != "|" && tokens[1] == "=" && tokens[2][0] == '/'))
	{
		ft_invalid_line(tokens);
	}
		
	std::string blockName = tokens[1];

	std::vector<Location> existing_locations = this->currentConfig->getLocation();

	for (size_t i = 0; i < existing_locations.size(); i++)
	{
		if (existing_locations[i].getBlockName() == blockName)
		{
			error("Error : Block name '" + blockName + "' already exists.");
		}
	}

	new_location.setBlockName(tokens[1]);

	if (tokens[1][0] == '/')
	{
		new_location.setBlockType("prefixe");
	}
	else if (tokens[1] == "=" && tokens[2][0] == '/')
	{
		new_location.setBlockType("equal");
	}

	for (size_t i = 3; i < tokens.size(); i++)
	{
		current_param.clear();

		while (i < tokens.size() && tokens[i] != "|")
		{
			current_param.push_back(tokens[i]);
			i++;
		}

		if (!current_param.empty())
		{
			ft_location_pages_dispatch(current_param, new_location);
		}
	}
	this->currentConfig->setBlockLocation(new_location);
}
