/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadConfigurationFile.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 20:01:59 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/30 16:03:58 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::readConfigurationFile(const char *arg)
{
	std::ifstream configFile(arg);
	if (!configFile.is_open())
	{
		error("Error: Could not open the configuration file [./webserv --help]");
	}

	std::string line;
	this->currentConfig = NULL;
	this->insideServerBlock = false;
	this->insideParamBlock = false;

	while (getline(configFile, line))
	{
		line.erase(0, line.find_first_not_of(" \t"));

		if (line.empty() || line[0] == '#')
			continue;

		if (this->insideServerBlock)
		{
			if (line.find("}") != std::string::npos)
			{
				this->insideServerBlock = false;
				if (this->currentConfig)
				{
					this->tab_serv.push_back(*this->currentConfig);
					delete this->currentConfig;
					this->currentConfig = NULL;
				}
			}
			else if (this->insideParamBlock && line.find("}") != std::string::npos)
			{
				this->insideParamBlock = false;
				ft_tokenizer(line);
			}
			else if (!this->insideParamBlock && line.find("{") != std::string::npos)
			{
				this->insideParamBlock = true;
				ft_tokenizer(line);
			}
			else
			{
				ft_tokenizer(line);
			}
		}
		else if (line.find("server {") != std::string::npos)
		{
			this->currentConfig = new ServerConfiguration();
			this->location_started = false;
			if (!this->currentConfig)
			{
				error("Error: Unable to allocate memory for ServerConfiguration");
			}
			this->insideServerBlock = true;
		}
	}

	if (this->insideServerBlock && this->currentConfig)
	{
		this->tab_serv.push_back(*this->currentConfig);
		delete this->currentConfig;
		this->currentConfig = NULL;
	}

	configFile.close();
}
