/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:12:04 by sguillot          #+#    #+#             */
/*   Updated: 2024/10/01 15:12:26 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string Response::autoIndex()
{
	std::stringstream html;
	std::string response;

	html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
	html << "<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	html << "<title>Index of " << this->_filePath << "</title>\n";
	html << "<style>\n";
	html << "body {font-family: 'Helvetica Neue', Arial, sans-serif; background: linear-gradient(135deg, rgb(0, 5, 71), rgb(81, 0, 87));";
	html << "background-size: 200% 200%; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center;";
	html << "height: 100vh; color: #fff; text-align: center; animation: gradientAnimation 20s ease infinite;}\n";
	html << "@keyframes gradientAnimation {0% {background-position: 0% 50%;} 50% {background-position: 100% 50%;} 100% {background-position: 0% 50%;}}\n";
	html << ".card {position: relative; border-radius: 15px; max-width: 600px; animation: fadeIn 1s ease-in-out; z-index: 1;}\n";
	html << ".card::before {content: \"\"; position: absolute; width: 104%; height: 102%; background-image: conic-gradient(rgb(0, 217, 255), rgb(247, 0, 255), rgb(255, 243, 135), rgb(0, 217, 255));";
	html << "top: 50%; left: 50%; translate: -50% -50%; z-index: -1; padding: 25px; border-radius: 15px; filter: blur(1.5rem); opacity: 0.5;}\n";
	html << ".card_background {padding: 50px; box-shadow: inset 0 0 100px #25044F; background-color: rgba(0, 5, 71, 35%); border-radius: 15px;}\n";
	html << "@keyframes fadeIn {0% {opacity: 0; transform: scale(0.8);} 100% {opacity: 1; transform: scale(1);}}\n";
	html << "h1 {font-size: 2.5rem; margin-bottom: 20px; text-transform: uppercase; letter-spacing: 2px; color: #fefefe; text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.7);}\n";
	html << "p {font-size: 1.2rem; line-height: 1.6; margin-bottom: 20px; text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.7);}\n";
	html << "li {display: flex; flex-direction: column; padding: 2rem; justify-content: center; align-items: center;}\n";
	html << "a {color: #fefefe; text-decoration: none; font-weight: bold; transition: color 0.1s ease, text-shadow 1s ease;}\n";
	html << "a:hover {color: #ffedc5; text-shadow: 0 0 10px rgba(255, 253, 237, 0.658);}\n";
	html << "</style>\n</head>\n<body>\n";
	html << "<main>\n<div class=\"card\">\n<div class=\"card_background\">\n";
	html << "<h1>Index of " << this->_filePath << "</h1>\n";
	html << "<ul>\n";

	DIR *dir;
	dir = opendir(this->_filePath.c_str());
	if (!dir)
	{
		_code = "403";
		return (ft_forbidden());
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string item = entry->d_name;
		if (item != ".")
		{
			html << "<li><a href=\"" << this->_filePath << "/" << item << "\">" << item << "</a></li>";
		}
	}

	closedir(dir);

	html << "</ul>\n</div>\n</div>\n</main>\n</body>\n</html>\n";

	response = firstHeader();
	response += "Content-Type: text/html \r\n";
	std::ostringstream oss;
	oss << html.str().size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() += "\r\n\r\n";
	response += html.str();

	return (response);
}
