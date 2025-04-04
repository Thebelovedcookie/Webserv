/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:29 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/10/01 23:59:58 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class ListeningSocket
{
	private:
		/*---------------------------------------------------------------*/
		/*                            ATTRIBUT                           */
		/*---------------------------------------------------------------*/
		int					port;
		std::string			_portStr;
		struct sockaddr_in	_server_address;
		struct protoent		*_proto;
		int					_socket_fd;
		std::string			ipAddress;

	public:
		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		ListeningSocket(std::string address, std::string port);
		~ListeningSocket();

		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/
		int					getSocket_fd() const;
		void				set_nonblocking(int sockfd);
		int					getPort() const;
		std::string			getPortStr() const;
		std::string			getIpAddress() const;
		/*---------------------------------------------------------------*/
		/*                         OVERLOADED                            */
		/*---------------------------------------------------------------*/
		bool				operator==(const ListeningSocket& other) const;
		bool				operator<(const ListeningSocket& other) const;
};
		
		std::ostream		&operator<<(std::ostream &Cout, ListeningSocket const &list);