/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setTokens.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 21:06:16 by sguillot          #+#    #+#             */
/*   Updated: 2024/10/01 13:40:22 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	Server::ft_invalid_line(std::vector<std::string> tokens)
{
    std::string error_message = "invalid line [./webserv --help]: ";
    for (int i = 0; (long unsigned int)i < tokens.size(); i++)
    {
        error_message += tokens[i];
        error_message += " ";
    }
    error(error_message);
}

void Server::ft_set_tokens(std::vector<std::string> tokens)
{
    if (this->insideParamBlock == true)
    {
        std::cout << "is inside param block" << std::endl;
    }
    else
    {
        ft_param_set_tokens(tokens);
    }
}

void Server::ft_param_set_tokens(std::vector<std::string> tokens)
{
    const int nb_param = 9;
    
    const char* param_array[nb_param] = {"listen", "server_name", "error_page",
                                         "client_max_body_size", "location", "index", "root",
                                         "allow_methods", "auto_index"};
    void (Server::*param_functions[nb_param])(std::vector<std::string>) = {
        &Server::ft_set_listen_param,
        &Server::ft_set_server_name_param,
        &Server::ft_set_error_page_param,
        &Server::ft_set_client_max_body_param,
        &Server::ft_set_location_param,
        &Server::ft_set_index_param,
        &Server::ft_set_root_param,
        &Server::ft_set_allowed_methods,
        &Server::ft_set_auto_index
    };

    for (int i = 0; i < nb_param; i++)
    {
        if (tokens.size() > 0 && std::strcmp(tokens[0].c_str(), param_array[i]) == 0)
        {
            (this->*param_functions[i])(tokens);
            return;
        }
    }
    
    if (tokens.size() > 0)
    {
        std::string error_message = "unknown parameter: " + (tokens.size() > 0 ? tokens[0] : "");
        error(error_message);
    }
}
