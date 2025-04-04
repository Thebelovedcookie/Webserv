/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:48 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/10/01 23:51:37 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"

class Client;

class Cgi
{
	private:
		/*---------------------------------------------------------------*/
		/*                          ATTRIBUT                             */
		/*---------------------------------------------------------------*/
		std::map<std::string, std::string>	_env;
		char								**_myEnvp;
		const char							*_path;
		std::string							_executer;

	public:
		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		Cgi();
		~Cgi();

		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/
		std::string							executeCgi();
		char								**conversionEnvFunc();
		void								setEnv(std::map<std::string, std::string> env);
		void								setPath(const char *path);
		void								error(std::string errorType);
		void								setExecuter(std::string executer);
		void								freeTab(char ***tab);
};

#endif