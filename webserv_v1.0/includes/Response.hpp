/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 22:07:42 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/10/02 13:31:15 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class Response
{
	private:
		/*---------------------------------------------------------------*/
		/*                   ATTRIBUT SERVER/LOCATION             		 */
		/*---------------------------------------------------------------*/
		std::vector<std::string>			_serverName;
		std::string							_blockName;
		std::string							_locationType;
		std::string							_alias;
		std::string							_root;
		std::string							_index;
		std::string							_cgiPath;
		std::string							_filePath;
		std::string							_code;
		std::string							_receivedLine;
		std::map<std::string, int>			_allowed_methods;
		std::map<int, std::string>			_errorPages;
		std::map<int, std::string>			_redirection;
		std::map<std::string, std::string>	_interpreterMap;
		std::map<std::string, std::string>	_mimePath;
		int									_clientMaxBodySize;
		int									_autoIndex;
		int									_status;
		int									_autoIndexUse;
		Client								*_client;

		/*---------------------------------------------------------------*/
		/*                   FONCTION GENERATE RESPONSE            		 */
		/*---------------------------------------------------------------*/
		std::string							ft_badRequest();
		std::string							ft_delete();
		std::string							ft_post();
		std::string							ft_get();
		std::string							ft_forbidden();
		std::string							readFileContent(std::string path);
		std::string							getMimeType();
		std::string							firstHeader();
		std::string							cgiExecution(std::string executer);
		std::string							dlSuccess();
		std::string							autoIndex();
		std::map<std::string, std::string>	createMimePath();
		std::map<std::string, std::string>	createEnvCgi();
		void								setFilePath(std::string root, std::string fileRequested);
		void								filePathFinder();
		void								dlFile();
		std::string							ft_notAllowed();
		void								saveFile(const std::string &filename, const std::string &data);

	public:
		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		Response(Client *client);
		~Response();
		
		std::string							generateResponse();
		/*---------------------------------------------------------------*/
		/*                            SETTER                             */
		/*---------------------------------------------------------------*/
		void								setInfo(ServerConfiguration *serv, Location location);
		void								setReceivedLine(std::string received);

		/*---------------------------------------------------------------*/
		/*                            GETTER                             */
		/*---------------------------------------------------------------*/
		std::string							getAlias() const;
		std::string							getRoot() const;
		std::string							getIndex() const;
		std::string							getFilePath() const;
		std::map<std::string, int>			getAllowedMethodsTab() const;
		std::map<int, std::string>			getErrorPage() const;
		std::map<int, std::string>			getRedirection() const;
		std::map<std::string, std::string>	getCgi() const;
		int									getAutoIndex() const;
		int									getClientMaxBodySize() const;
};

std::ostream						&operator<<(std::ostream &Cout, Response const &response);