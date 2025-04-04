/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:44 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/10/02 13:08:29 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CLIENT_HPP
#define CLIENT_HPP
#include "webserv.hpp"

class Client
{
	private:
		/*---------------------------------------------------------------*/
		/*                            ATTRIBUT                           */
		/*---------------------------------------------------------------*/	
		std::string	_method;
		std::string	_ipAdress;
		std::string	_httpAcceptMime;
		std::string	_httpAcceptLanguage;
		std::string	_httpUserAgent;
		std::string	_httpReferer;
		std::string	_contentType;
		std::string	_contentLength;
		std::string	_boundary;
		std::string	_host;
		std::string	_path;
		int			_currentFd;
		std::string	_ipAdressConnexion;
		std::string	_portConnexion;
		std::string	_request;
		bool		_headerComplete;
		bool		_requestComplete;
		int			_totalRead;
		
	public:
		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		Client();
		~Client();

		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/
		void		setInfo(std::string info);
		void		error(std::string errorType);

		/*---------------------------------------------------------------*/
		/*                            GETTER                             */
		/*---------------------------------------------------------------*/
		std::string getMethod() const;
		std::string getIpAdress() const;
		std::string getAcceptMime() const;
		std::string getAcceptLanguage() const;
		std::string getUserAgent() const;
		std::string getReferer() const;
		std::string getContentType() const;
		std::string getContentLength() const;
		std::string getBoundary() const;
		std::string	getHost() const;
		std::string	getPath() const;
		std::string	getIpAdressConnexion() const;
		std::string	getPortStr() const;
		int			getCurrentFd() const;
		std::string	getRequest() const;
		bool		getHeaderComplete() const;
		bool		getRequestComplete() const;
		int			getTotalRead() const;

		/*---------------------------------------------------------------*/
		/*                            SETTER                             */
		/*---------------------------------------------------------------*/
		void		setMethod(std::string method);
		void		setIpAddress(std::string ipAdress);
		void		setAcceptMime(std::string httpAcceptMime);
		void		setAcceptLanguage(std::string httpAcceptLanguage);
		void		setUserAgent(std::string userAgent);
		void		setReferer(std::string referer);
		void		setContentType(std::string contentType);
		void		setContentLength(std::string contentLength);
		void		setBoundary(std::string boundary);
		void		setHost(std::string host);
		void		setCurrentFd(int currentFd);
		void		setIpAdressConnexion(std::string ipadress);
		void		setPortStr(std::string port);
		void		setRequest(std::string toAppend);
		void		setHeaderComplete(bool value);
		void		setRequestComplete(bool value);
		void		setTotalRead(int n);
};

std::ostream	&operator<<(std::ostream &Cout, Client const &i);
#endif