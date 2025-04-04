/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:41:27 by sguillot          #+#    #+#             */
/*   Updated: 2024/10/01 23:56:04 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class ListeningSocket;

class ServerConfiguration
{
	private:
		/*---------------------------------------------------------------*/
		/*                            ATTRIBUT                           */
		/*---------------------------------------------------------------*/

		std::vector<std::string>				_serverName;
		std::multimap<std::string, std::string>	_portList;
		std::string								_root;
		std::string								_index;
		int										_clientMaxBodySize;
		std::map<std::string, std::string>		_interpreterMap;
		std::map<std::string, int>				_allowed_methods;
		std::map<int, std::string>				_errorPages;
		int										_autoIndex;
		std::vector<Location>					_location;
		std::map<std::string, std::string>		_pathInfoMime;
		std::string								_strPort;

	public:
		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		ServerConfiguration();
		~ServerConfiguration(void);
		
		/*---------------------------------------------------------------*/
		/*                            SETTER                             */
		/*---------------------------------------------------------------*/
		void									setListen(std::string address, std::string port);
		void									setServerName(std::string str);
		void									setErrorPages(int code, std::string str);
		void									setClientMaxBodySize(std::string str);
		void									setRoot(std::string str);
		void									setIndex(std::string str);
		void									setAllowedMethods(std::string method, int code);
		void									setInterpreterMap(std::string extension, std::string location);
		void									setAutoIndex(std::string str);
		void									setBlockLocation(Location new_location);

		/*---------------------------------------------------------------*/
		/*                            GETTER                             */
		/*---------------------------------------------------------------*/
		int										getClientMaxBodySize(void) const;
		int										getAutoIndex() const;
		std::string								getStrPort(void) const;
		std::string								getRoot(void) const;
		std::string								getErrorPage(int code) const;
		std::string								getIndex(void) const;
		std::string								getAutoIndexStr() const;
		std::vector<Location>					getLocation() const;
		std::vector<std::string>				getServerName(void) const;
		std::map<std::string, int>				getAllowedMethods(void)const;
		std::map<int, std::string>				getErrorPages(void) const;
		std::map<std::string, std::string>		getInterpreterMap(void) const;
		std::map<std::string, std::string>		getInfoMime(void) const;
		std::multimap<std::string, std::string>	getPortList(void) const;

		/*---------------------------------------------------------------*/
		/*                            UTILS                              */
		/*---------------------------------------------------------------*/
		void									error(std::string errorType);
};

std::ostream	&operator<<(std::ostream &Cout, ServerConfiguration const &i);
