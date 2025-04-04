/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 20:07:45 by sguillot          #+#    #+#             */
/*   Updated: 2024/09/18 13:58:13 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void Server::ft_tokenizer(std::string line)
{
	std::vector<std::string> tokens;
	std::string buffer;
	std::string::const_iterator line_copy;

	for (line_copy = line.begin(); line_copy != line.end(); ++line_copy)
	{
		if (!std::isspace(*line_copy))
		{
			buffer += *line_copy;
		}
		else if (!buffer.empty())
		{
			tokens.push_back(buffer);
			buffer.clear();
		}
	}

	if (!buffer.empty())
	{
		tokens.push_back(buffer);
	}

	ft_set_tokens(tokens);
}