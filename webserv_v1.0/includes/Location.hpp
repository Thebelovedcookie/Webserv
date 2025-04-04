/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Creted: 2024/09/20 11:51:44 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/09/20 14:17:32 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class Location
{
	private:
		std::string							_blockName;
		std::string							_blockType;
		std::string                 		_alias;
		std::string                 		_root;
		int                 				_clientMaxBodySize;
		int                         		_autoindex;
		std::string                 		_path_info;
		std::string                 		_index;
		std::map<std::string, std::string>	_cgi;
		std::map<int, std::string>			_error_page;
		std::map<int, std::string>			_redirection;
		std::map<std::string, int>  		_allowed_methods;

	public:
		Location(/* args */);
		~Location();

		Location							&operator=(Location const &obj);

		/*---------------------------------------------------------------*/
		/*                            SETTER                             */
		/*---------------------------------------------------------------*/
		void								setBlockName(std::string blockName);
		void								setBlockType(std::string blockType);
		void								setAlias(std::string alias);
		void								setRoot(std::string root);
		void								setClientMaxBodySize(int clientMaxBodySize);
		void								setAutoIndex(int autoIndex);
		void								setIndex(std::string index);
		void								setCgi(std::string first, std::string second);
		void								setErrorPage(int code, std::string errorPage);
		void								setRedirection(int code, std::string to);
		void								setAllowedMethods(std::string whichOne, int allow);


		/*---------------------------------------------------------------*/
		/*                            GETTER                             */
		/*---------------------------------------------------------------*/
		std::string							getBlockName() const;
		std::string							getBlockType() const;
		std::string							getAlias() const;
		std::string							getRoot() const;
		int									getClientMaxBodySize() const;
		int									getAutoIndex() const;
		std::string							getPathInfo() const;
		std::string							getIndex() const;
		std::map<std::string, std::string>	getCgi() const;
		std::map<int, std::string>			getErrorPage() const;
		std::map<int, std::string>			getRedirection() const;
		int									getAllowedMethods(std::string wichOne);
		std::map<std::string, int>			getAllowedMethodsTab() const;
};

std::ostream 	&operator<<(std::ostream &Cout, Location const &list);