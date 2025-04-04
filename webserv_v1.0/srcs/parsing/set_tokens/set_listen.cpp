/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_listen.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:21:41 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/25 20:41:18 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_checkIp(const std::string &ip_str)
{
	    if (ip_str.size() < 7 || ip_str.size() > 15
        || std::strncmp("10", ip_str.c_str(), 2) == 0
        || std::strncmp("192.168", ip_str.c_str(), 7) == 0
        || std::strncmp("0.0.0.0", ip_str.c_str(), 7) == 0
        || std::strncmp("255.255.255.255", ip_str.c_str(), 15) == 0)
    {
        error("Error: Forbiden ip address : " + ip_str);
    }

    int num = 0;
    int point_counter = 0;
    int segment_length = 0;

    for (size_t i = 0; i < ip_str.size(); ++i)
    {
        char ch = ip_str[i];
        if (ch == '.')
        {
            if (num < 0 || num > 255 || segment_length == 0 || segment_length > 3)
            {
                error("Error: Forbiden ip address : " + ip_str);
            }
            num = 0;
            segment_length = 0;
            ++point_counter;
        }
        else if (ch >= '0' && ch <= '9')
        {
            num = num * 10 + (ch - '0');
            ++segment_length;
        }
        else
        {
            error("Error: Forbiden ip address : " + ip_str);
        }
    }

    if (num < 0 || num > 255 || point_counter != 3
        || segment_length == 0 || segment_length > 3)
    {
        error("Error: Forbiden ip address : " + ip_str);
    }
}

void Server::ft_checkPort(const std::string port_str)
{
	for (size_t i = 0; i < port_str.size(); ++i)
	{
		if (!std::isdigit(port_str[i]))
		{
			error("Error: non-numeric character in port number: " + port_str);
		}
	}

	std::string num_str = port_str;
	char *end;
	long port = std::strtol(num_str.c_str(), &end, 10);

	if (*end != '\0')
	{
		error("Error: invalid argument (listen): " + port_str);
	}
	else if (port < 0 || port > 65535)
	{
		error("Error: out of bounds port (listen argument): " + port_str);
	}
	else if (port <= 1023					 /* Critical system 0-1023 */
			 || port == 1080					 /* SOCKS Proxy 1080 */
			 || (port >= 1433 && port <= 1434) /* Microsoft SQL server 1433-1434 */
			 || port == 1521					 /* Oracle Database 1521 */
			 || port == 1723					 /* PPTP (Point-to-Point Tunneling Protocol) 1723 */
			 || port == 3306					 /* MySQL 3306 */
			 || port == 5432					 /* PostgreSQL 5432 */
			 || (port >= 5900 && port <= 5901) /* VNC (Virtual Network Computing) 5900-5901 */
			 || port == 6379					 /* Redis 6379 */
			 || (port >= 6660 && port <= 6669) /* IRC 6660-6669 */
			 || port == 27017 				 /* MongoDB 27017 */)
	{
		error("Error: forbiden port (listen argument): " + port_str);
	}
}

void Server::ft_set_listen_param(std::vector<std::string> tokens)
{
	if (tokens.size() != 2 || tokens[1].empty() || this->location_started)
	{
		ft_invalid_line(tokens);
	}

	std::string ip = "";
	std::string	port = "";

	size_t colon_separator = tokens[1].find(":");
	if (colon_separator != std::string::npos)
	{
		ip = tokens[1].substr(0, colon_separator);
		port = tokens[1].substr(colon_separator + 1, tokens[1].size());
	}
	else
	{
		port = tokens[1];
	}

	if (ip != "")
	{
		ft_checkIp(ip);
	}
	
	if (port == "")
	{
		error("Error : No port : " + tokens[1]);
	}
	ft_checkPort(port);
	
	this->currentConfig->setListen(ip, port);
}
