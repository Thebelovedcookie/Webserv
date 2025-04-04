/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:29:58 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/10/02 13:08:50 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

Client::Client()
{
	this->_httpAcceptLanguage = "";
	this->_httpAcceptMime = "";
	this->_httpReferer = "";
	this->_httpUserAgent = "";
	this->_ipAdress = "";
	this->_method = "";
	this->_contentLength = "";
	this->_contentType = "";
	this->_boundary = "";
	this->_path = "";
	this->_host = "";
	this->_currentFd = -1;
	this->_ipAdressConnexion = "";
	this->_portConnexion = "";
	this->_request = "";
	this->_headerComplete = false;
	this->_requestComplete = false;
	this->_totalRead = 0;
}

Client::~Client()
{
}

/*----------------------------------------------------------------------------*/
/*                              METHOD/SERVER                                 */
/*----------------------------------------------------------------------------*/


void Client::setInfo(std::string info)
{	
	/****METHOD****/
	size_t methodEnd = info.find(" ");
	if (methodEnd != std::string::npos)
		this->setMethod(info.substr(0, methodEnd));

	/****URI****/
	size_t path_start = info.find('/');
	if (path_start != std::string::npos)
	{
		size_t path_end = info.find(' ', path_start);
		if (path_end != std::string::npos)
			this->_path = info.substr(path_start, path_end - path_start);
	}

	/****HOST****/
	size_t host = info.find("Host: ");
	if (host != std::string::npos)
	{
		host += 6;
		size_t endHost = info.find("\r\n", host);
		if (endHost != std::string::npos)
		{
			std::string tmphost = info.substr(host, (endHost - host));
			size_t pos = tmphost.find(':');
    		if (pos != std::string::npos)
        		this->setHost(tmphost.substr(0, pos));
			else
				this->setHost(tmphost);
		}
    }

    /****USER-AGENT****/
	size_t userAgent = info.find("User-Agent: ");
	if (userAgent != std::string::npos)
	{
		userAgent += 12;
		size_t endUserAgent = info.find("\r\n", userAgent);
		if (endUserAgent != std::string::npos)
			this->setUserAgent(info.substr(userAgent, (endUserAgent - userAgent)));
	}

 	/****ACCEPT****/
	size_t accept = info.find("Accept: ");
	if (accept != std::string::npos)
	{
		accept += 8;
		size_t	endAccept = info.find("\r\n", accept);
		if (endAccept != std::string::npos)
			this->setAcceptMime(info.substr(accept, (endAccept - accept)));
	}

	/****ACCEPT-LANGUAGE****/
	size_t acceptLanguage = info.find("Accept-Language: ");
	if (acceptLanguage != std::string::npos)
	{
		acceptLanguage += 17;
		size_t endAcceptLanguage = info.find("\r\n", acceptLanguage);
		if (endAcceptLanguage != std::string::npos)
			this->setAcceptLanguage(info.substr(acceptLanguage, (endAcceptLanguage - acceptLanguage)));
	}

	/****REFERER****/
	size_t referer = info.find("Referer: ");
	if (referer != std::string::npos)
	{
		referer += 9;
		size_t endReferer = info.find("\r\n", referer);
		if (endReferer != std::string::npos)
			this->setReferer(info.substr(referer, (endReferer - referer)));
	}

	/****CONTENT-TYPE****/
	size_t contentType = info.find("Content-Type: ");
	if (contentType != std::string::npos)
	{
		contentType += 14;
		size_t endContentType = info.find(";", contentType);
		if (endContentType != std::string::npos)
			this->setContentType(info.substr(contentType, (endContentType - contentType)));
	}

	/****CONTENT-LENGTH****/
	size_t contentLength = info.find("Content-Length: ");
	if (contentLength != std::string::npos)
	{
		contentLength += 16;
		size_t endContentLength = info.find("\r\n", contentLength);
		if (endContentLength != std::string::npos)
			this->setContentLength(info.substr(contentLength, (endContentLength - contentLength)));
	}

	/****BOUNDARY****/
	size_t boundary = info.find("boundary=");
	if (boundary != std::string::npos)
	{
		boundary += 9;
		size_t endBoundary = info.find("\r\n", boundary);
		if (endBoundary != std::string::npos)
			this->setBoundary(info.substr(boundary, (endBoundary - boundary)));
	}
}

/*----------------------------------------------------------------------------*/
/*                                   ACCESSOR                                 */
/*----------------------------------------------------------------------------*/

std::string	Client::getMethod() const {
	return (this->_method);
}

std::string	Client::getIpAdress() const {
	return (this->_ipAdress);
}

std::string	Client::getAcceptMime() const{
	return (this->_httpAcceptMime);
}

std::string	Client::getAcceptLanguage() const {
	return (this->_httpAcceptLanguage);
}

std::string	Client::getUserAgent() const {
	return (this->_httpUserAgent);
}
std::string	Client::getReferer() const {
	return (this->_httpReferer);
}

std::string	Client::getContentType() const {
	return (this->_contentType);
}

std::string	Client::getContentLength() const {
	return (this->_contentLength);
}

std::string	Client::getBoundary() const {
	return (this->_boundary);
}

std::string	Client::getHost() const {
	return (this->_host);
}

int Client::getCurrentFd() const
{
	return (this->_currentFd);
}

std::string Client::getRequest() const
{
    return (this->_request);
}

bool Client::getHeaderComplete() const
{
    return (this->_headerComplete);
}

bool Client::getRequestComplete() const
{
    return (this->_requestComplete);
}

int Client::getTotalRead() const
{
    return (this->_totalRead);
}

std::string Client::getPath() const
{
	return (this->_path);
}

std::string Client::getIpAdressConnexion() const
{
    return (this->_ipAdressConnexion);
}

std::string Client::getPortStr() const
{
    return (this->_portConnexion);
}

/*----------------------------------------------------------------------------*/

void	Client::setMethod(std::string method) {
	this->_method = method;
}

void	Client::setIpAddress(std::string ipAdress) {
	this->_ipAdress = ipAdress;
}

void	Client::setAcceptMime(std::string httpAcceptMime) {
	this->_httpAcceptMime = httpAcceptMime;
}

void	Client::setAcceptLanguage(std::string httpAcceptLanguage) {
	this->_httpAcceptLanguage = httpAcceptLanguage;
}

void	Client::setUserAgent(std::string userAgent) {
	this->_httpUserAgent = userAgent;
}

void	Client::setReferer(std::string referer) {
	this->_httpReferer = referer;
}

void	Client::setContentType(std::string contentType) {
	this->_contentType = contentType;
}

void	Client::setContentLength(std::string contentLength) {
	this->_contentLength = contentLength;
}

void	Client::setBoundary(std::string boundary) {
	this->_boundary = boundary;
}

void	Client::setHost(std::string host)
{
	this->_host = host;
}

void Client::setCurrentFd(int currentFd)
{
	this->_currentFd = currentFd;
}

void Client::setIpAdressConnexion(std::string ipadress)
{
	this->_ipAdressConnexion = ipadress;
}

void Client::setPortStr(std::string port)
{
	this->_portConnexion = port;
}

void Client::setRequest(std::string toAppend)
{
	this->_request += toAppend;
}

void Client::setHeaderComplete(bool value)
{
	this->_headerComplete = value;
}

void Client::setRequestComplete(bool value)
{
	this->_requestComplete = value;
}

void Client::setTotalRead(int n)
{
	_totalRead += n;
}

/*----------------------------------------------------------------------------*/
/*                                  UTILS                                     */
/*----------------------------------------------------------------------------*/

void		Client::error(std::string errorType)
{
	throw(std::runtime_error(errorType));
}

std::ostream &operator<<(std::ostream &Cout, Client const &i)
{
	Cout << "info client	: " << std::endl;
	Cout << "Method		: " << i.getMethod() << std::endl;
	Cout << "Ip adress	: " << i.getIpAdress() << std::endl;
	Cout << "User-Agent	: " << i.getUserAgent() << std::endl;
	Cout << "Referer		: " << i.getReferer() << std::endl;
	Cout << "language	: " << i.getAcceptLanguage() << std::endl;
	Cout << "Mime		: " << i.getAcceptMime() << std::endl;
	Cout << "Content-Length : " << i.getContentLength() << std::endl;
	Cout << "Content-Type	: " << i.getContentType() << std::endl;
	
	return (Cout);
}
