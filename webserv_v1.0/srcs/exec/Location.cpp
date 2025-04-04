/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 12:40:40 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/10/01 17:33:55 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Location.hpp"

/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

Location::Location(/* args */)
{
	this->_blockName = "";
	this->_blockType = "";
	this->_alias = "";
	this->_root = "";
	this->_clientMaxBodySize = -1;
	this->_autoindex = 0;
	this->_path_info = "";
	this->_index = "";
}

Location::~Location()
{
	this->_cgi.clear();
	this->_error_page.clear();
	this->_redirection.clear();
	this->_allowed_methods.clear();
}

/*----------------------------------------------------------------------------*/
/*                                   SETTER                                   */
/*----------------------------------------------------------------------------*/

Location &Location::operator=(Location const &obj)
{
    this->_blockName = obj._blockName;
	this->_blockType = obj._blockType;
	this->_alias = obj._alias;
	this->_root = obj._root;
	this->_clientMaxBodySize = obj._clientMaxBodySize;
	this->_autoindex = obj._autoindex;
	this->_path_info = obj._path_info;
	this->_index = obj._index;
	this->_cgi.clear();
	std::map<std::string, std::string> tmp = obj.getCgi();
	std::map<std::string, std::string>::iterator it = tmp.begin();
	for (; it != tmp.end(); it++)
	{
		this->_cgi[it->first] = it->second;
	}
	this->_error_page.clear();
	this->_error_page = obj._error_page;
	this->_redirection.clear();
	this->_redirection = obj._redirection;
	this->_allowed_methods.clear();
	std::map<std::string, int> tmp2 = obj.getAllowedMethodsTab();
	std::map<std::string, int>::iterator it2 = tmp2.begin();
	for (; it2 != tmp2.end(); it2++)
	{
		this->_cgi[it2->first] = it2->second;
	}
	return (*this);
}

void Location::setBlockName(std::string blockName)
{
    this->_blockName = blockName;
}

void Location::setBlockType(std::string blockType) {
	this->_blockType = blockType;
}

void Location::setAlias(std::string alias) {
	this->_alias = alias;
}

void Location::setRoot(std::string root) {
	this->_root = root;
}

void Location::setClientMaxBodySize(int clientMaxBodySize) {
	this->_clientMaxBodySize = clientMaxBodySize;
}

void Location::setAutoIndex(int autoIndex) {
	this->_autoindex = autoIndex;
}

void Location::setIndex(std::string index) {
	this->_index = index;
}

void Location::setCgi(std::string first, std::string second) {
	this->_cgi[first] = second;
}

void Location::setErrorPage(int code, std::string errorPage) {
	this->_error_page[code] = errorPage;
}

void Location::setRedirection(int code, std::string to) {
	this->_redirection[code] = to;
}

void Location::setAllowedMethods(std::string whichOne, int allow) {
	this->_allowed_methods[whichOne] = allow;
}

/*----------------------------------------------------------------------------*/
/*                                   GETTER                                   */
/*----------------------------------------------------------------------------*/

std::string Location::getBlockName() const {
	return (this->_blockName);
}

std::string Location::getBlockType() const {
	return (this->_blockType);
}

std::string Location::getAlias() const {
	return (this->_alias);
}

std::string Location::getRoot() const {
	return (this->_root);
}

std::string Location::getPathInfo() const {
	return (this->_path_info);
}

std::string Location::getIndex() const {
	return (this->_index);
}

std::map<std::string, std::string> Location::getCgi() const {
	return (this->_cgi);
}

std::map<int, std::string> Location::getErrorPage() const {
	return (this->_error_page);
}

std::map<int, std::string> Location::getRedirection() const {
	return (this->_redirection);
}

std::map<std::string, int> Location::getAllowedMethodsTab() const {
	return (this->_allowed_methods);
}

int Location::getAllowedMethods(std::string whichOne) {
	std::map<std::string, int>::iterator it = _allowed_methods.find(whichOne);
	if (it != _allowed_methods.end()) {
        return (it->second);
    }
	return (0);
}

int Location::getClientMaxBodySize() const {
	return (this->_clientMaxBodySize);
}

int Location::getAutoIndex() const {
	return (this->_autoindex);
}

/*----------------------------------------------------------------------------*/
/*                                  UTILS                                     */
/*----------------------------------------------------------------------------*/

std::ostream &operator<<(std::ostream &Cout, Location const &location)
{
	Cout << "alias	: " << location.getAlias() << std::endl;
	Cout << "blockname		: " << location.getBlockName() << std::endl;
	Cout << "blocktype		: " << location.getBlockType() << std::endl;
	return (Cout);
}