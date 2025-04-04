/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 22:12:59 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/10/02 13:32:18 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Response.hpp"

/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

Response::Response(Client *client)
{
	this->_locationType = "";
	this->_alias = "";
	this->_root = "";
	this->_clientMaxBodySize = -1;
	this->_autoIndex = -1;
	this->_index = "";
	this->_blockName = "";
	this->_client = client;
	this->_mimePath = createMimePath();
	this->_code = "";
	this->_autoIndexUse = 0;
	this->_receivedLine = "";
}

Response::~Response()
{
	this->_interpreterMap.clear();
	this->_allowed_methods.clear();
	this->_errorPages.clear();
	this->_redirection.clear();
}

/*----------------------------------------------------------------------------*/
/*                              METHOD/SERVER                                 */
/*----------------------------------------------------------------------------*/

void Response::setInfo(ServerConfiguration *serv, Location location)
{
	std::vector<std::string> tab = serv->getServerName();
	std::vector<std::string>::iterator it = tab.begin();
	for (; it != tab.end(); it++)
	{
		this->_serverName.push_back(*it);
	}
	if (location.getBlockName() != "")
	{
		this->_locationType = location.getBlockType();
		this->_blockName = location.getBlockName();
		this->_alias = location.getAlias();

		if (location.getRoot() != "")
		{
			this->_root = location.getRoot();
		}
		else
		{
			this->_root = serv->getRoot();
		}

		if (location.getIndex() != "")
			this->_index = location.getIndex();
		else
			this->_index = serv->getIndex();

		if (location.getClientMaxBodySize() != -1)
			this->_clientMaxBodySize = location.getClientMaxBodySize();
		else
			this->_clientMaxBodySize = serv->getClientMaxBodySize();

		if (!location.getCgi().empty())
			this->_interpreterMap = location.getCgi();
		else
			this->_interpreterMap = serv->getInterpreterMap();

		if (!location.getAllowedMethodsTab().empty())
		{
			this->_allowed_methods["GET"] = location.getAllowedMethods("GET");
			this->_allowed_methods["POST"] = location.getAllowedMethods("POST");
		}
		else
			this->_allowed_methods = serv->getAllowedMethods();

		if (!location.getErrorPage().empty())
			this->_errorPages = location.getErrorPage();
		else
			this->_errorPages = serv->getErrorPages();

		if (location.getAutoIndex() != -1)
			this->_autoIndex = location.getAutoIndex();
		else
			this->_autoIndex = serv->getAutoIndex();

		if (!location.getRedirection().empty())
			this->_redirection = location.getRedirection();
	}
	else
	{
		this->_root = serv->getRoot();
		this->_index = serv->getIndex();
		this->_clientMaxBodySize = serv->getClientMaxBodySize();
		this->_interpreterMap = serv->getInterpreterMap();
		this->_allowed_methods = serv->getAllowedMethods();
		this->_errorPages = serv->getErrorPages();
		this->_autoIndex = serv->getAutoIndex();
	}
}

void Response::setReceivedLine(std::string received)
{
	this->_receivedLine = received;
}

std::string Response::ft_notAllowed()
{
	char *tmp = getcwd(NULL, 0);
	std::string root(tmp, strlen(tmp));
	free(tmp);
	std::string content = readFileContent(root + "/www/error_pages/405.html");

	std::string response = "HTTP/1.1 405 Method Not Allowed\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	response += content;
	return (response);
}

std::string Response::generateResponse()
{
	if (this->_allowed_methods[this->_client->getMethod()] != 1)
	{
		_code = "403";
		return (ft_notAllowed());
	}
	filePathFinder();
	if (_autoIndexUse == 1)
	{
		Server::log("Auto Index Mode activated.", 1);
		this->_filePath = this->_root;
		return (autoIndex());
	}
	if (_client->getContentLength() != "" && this->_interpreterMap.empty())
	{
		if (atoi(_client->getContentLength().c_str()) <= _clientMaxBodySize)
		{
			dlFile();
			return (dlSuccess());
		}
		else
			Server::log("Content-Length Higher than Max Body Size.", 2);
	}
	struct stat buf;
	if (stat(_filePath.c_str(), &buf) == 0)
	{
		if (S_ISREG(buf.st_mode) && access(_filePath.c_str(), F_OK) == 0 && !this->_interpreterMap.empty())
		{
			size_t ext = this->_filePath.rfind(".");
			if (ext != std::string::npos)
			{
				std::string _extension = this->_filePath.substr(ext);
				std::map<std::string, std::string>::iterator it = this->_interpreterMap.begin();
				for (; it != this->_interpreterMap.end(); it++)
				{
					if (_extension == it->first)
					{
						Server::log("Cgi Execution.", 1);
						return (cgiExecution(it->second));
					}
				}
			}
		}
		else if (S_ISDIR(buf.st_mode) && this->_autoIndex == 1 && _client->getMethod() != "DELETE")
			return (autoIndex());
	}
	else
	{
		_code = "404";
		Server::log("URI not Found.", 1);
		return (ft_badRequest());
	}

	if (this->_client->getMethod() == "GET")
		return (ft_get());
	else if (this->_client->getMethod() == "POST")
		return (ft_post());
	else if (this->_client->getMethod() == "DELETE")
		return (ft_delete());
	else
		return (ft_badRequest());
}

std::string Response::dlSuccess()
{
	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/html\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	return (response);
}

void Response::dlFile()
{
	std::string file_name;
	std::string body_content;
	this->_receivedLine = _client->getRequest();
	size_t filename = (this->_receivedLine).find("filename=\"");
	if (filename != std::string::npos)
	{
		filename += 10;
		size_t endFilename = (this->_receivedLine).find("\"", filename);
		if (endFilename != std::string::npos)
		{
			file_name = (this->_receivedLine).substr(filename, (endFilename - filename));

			size_t body = (this->_receivedLine).find("\r\n\r\n", endFilename);
			if (body != std::string::npos)
			{
				body += 4;
				size_t endbody = (this->_receivedLine).find(_client->getBoundary(), body);
				if (endbody != std::string::npos)
				{
					body_content = (this->_receivedLine).substr(body, (endbody - body));
					(this->_receivedLine).erase(body, (endbody - body));
					saveFile(this->_root + "/" + file_name, body_content);
				}
				else
					Server::log("The request body cannot be found.", 2);
			}
			else
				Server::log("The request body cannot be found.", 2);
		}
		else
			Server::log("Filename cannot be found.", 2);
	}
	else
		Server::log("Filename cannot be found.", 2);
}

void Response::saveFile(const std::string &filename, const std::string &data)
{
	std::ofstream file(filename.c_str(), std::ios::binary);
	if (file.is_open())
	{
		file.write(data.c_str(), data.size());
		file.close();
		Server::log("File " + filename + " saved successfully.", 1);
	}
	else
	{
		Server::log("Failed to open file " + filename + ".", 2);
	}
}

std::string Response::firstHeader()
{
	if (_code == "301")
		return ("HTTP/1.1 301 Moved Permanently\r\n");
	else if (_code == "302")
		return ("HTTP/1.1 302 Moved Temporarily\r\n");
	return ("HTTP/1.1 200 OK\r\n");
}

std::map<std::string, std::string> Response::createEnvCgi()
{
	std::map<std::string, std::string> env;
	if (_client->getMethod() == "POST")
	{
		env["CONTENT_TYPE"] = _client->getContentType(); 
		env["CONTENT_LENGTH"] = _client->getContentLength();
	}
	// SERVEUR_VAR
	env["SERVER_SOFTWARE"] = std::string("Webserv/1.0");
	env["SERVER_NAME"] = *this->_serverName.begin();
	env["GATEWAY_INTERFACE"] = std::string("CGI/1.1");

	// REQUEST_VAR
	env["SERVER_PROTOCOL"] = std::string("HTTP/1.1");
	env["SERVER_PORT"] = _client->getPortStr();
	env["REQUEST_METHOD"] = _client->getMethod();
	env["PATH_INFO"] = _client->getPath();
	env["PATH_TRANSLATED"] = _filePath;
	env["SCRIPT_NAME"] = _filePath;
	env["QUERY_STRING"] = std::string("");
	env["REMOTE_HOST"] = std::string("");
	env["REMOTE_ADDR"] = _client->getIpAdress();

	// CLIENT_VAR
	env["HTTP_ACCEPT"] = _client->getAcceptMime();
	env["HTTP_ACCEPT_LANGUAGE"] = _client->getAcceptLanguage();
	env["HTTP_USER_AGENT"] = _client->getUserAgent();
	env["HTTP_COOKIE"] = std::string("");
	env["HTTP_REFERER"] = _client->getReferer();
	return (env);
}

std::string Response::cgiExecution(std::string executer)
{
	Cgi *cgi = new Cgi();
	cgi->setExecuter(executer);
	cgi->setPath(_filePath.c_str());
	cgi->setEnv(createEnvCgi());
	std::string content = cgi->executeCgi();

	std::string mimeType = getMimeType();

	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: " + mimeType + "\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	response += content;

	delete cgi;
	return (response);
}

void Response::filePathFinder()
{
	if (this->_locationType != "" && this->_locationType == "equal")
	{
		_code = "200";
		_filePath = this->_root + this->_client->getPath();
	}
	else if (this->_alias != "" && this->_client->getPath() == this->_blockName)
	{
		_code = "200";
		size_t stBlock = this->_client->getPath().find(this->_blockName);

		std::string path_tmp = this->_client->getPath();
		if (stBlock != std::string::npos)
			path_tmp.replace(stBlock, this->_blockName.size(), this->_alias);
		_filePath = this->_root + path_tmp;
	}
	else if (!this->_redirection.empty() && this->_client->getPath() == this->_blockName)
	{
		std::stringstream ss;
		ss << this->_redirection.begin()->first;
		std::string str = ss.str();
		_code = str;
		_filePath = this->_redirection.begin()->second;
	}
	else if (this->_client->getPath() == "/")
	{
		_code = "200";
		if (_index != "")
			_filePath = this->_root + this->_index;
		else if (_autoIndex == 1)
		{
			_autoIndexUse = 1;
			return;
		}
		else
			_filePath = "";
	}
	else
	{
		_code = "200";
		std::string tmp = this->_root + this->_client->getPath();
		if (access(tmp.c_str(), F_OK) != 0)
		{
			if (this->_index != "")
			{
				std::vector<std::string>::iterator it = this->_serverName.begin();
				for (; it != _serverName.end(); it++)
				{
					if (_client->getPath() == "/" + *it)
						_filePath = this->_root + this->_index;
				}
			}
			else if (_autoIndex == 1)
			{
				std::vector<std::string>::iterator it = this->_serverName.begin();
				for (; it != _serverName.end(); it++)
				{
					if (_client->getPath() == "/" + *it)
					{
						_autoIndexUse = 1;
						return;
					}
				}
				this->_filePath = _client->getPath();
			}
		}
		if (_filePath == "")
		{
			_filePath = this->_root + this->_client->getPath();
		}
	}
}

std::string Response::ft_get()
{
	Server::log("Server's received a GET request.", 1);

	if (access(this->_filePath.c_str(), F_OK) != 0)
	{
		std::string response = "HTTP/1.1 404 Not Found\r\n";
		response += "Content-Type: text/html\r\n";
		std::string content = "File not found";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
		response += content;
		return (response);
	}

	std::string content = readFileContent(this->_filePath);

	if (content.empty())
	{
		std::string response = "HTTP/1.1 500 Internal Server Error\r\n";
		response += "Content-Type: text/html\r\n";
		std::string error_content = "Error reading file content";
		std::ostringstream oss;
		oss << error_content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
		response += error_content;
		return (response);
	}

	Server::log("The file requested \"" + this->_filePath + "\" was found.", 1);

	std::string mimeType = getMimeType();
	std::string response = firstHeader();
	response += "Content-Type: " + mimeType + "\r\n";

	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	response += content;

	return (response);
}

/*response to a POST request*/
std::string Response::ft_post()
{
	Server::log("Server's received a POST request.", 1);

	if (access(this->_filePath.c_str(), F_OK) != 0)
	{
		std::string response = "HTTP/1.1 404 Not Found\r\n";
		response += "Content-Type: text/html\r\n";
		std::string content = "File not found";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
		response += content;
		return (response);
	}

	std::string content = readFileContent(this->_filePath);

	if (content.empty())
	{
		std::string response = "HTTP/1.1 500 Internal Server Error\r\n";
		response += "Content-Type: text/html\r\n";
		std::string error_content = "Error reading file content";
		std::ostringstream oss;
		oss << error_content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
		response += error_content;
		return (response);
	}

	std::string mimeType = getMimeType();
	std::string response = firstHeader();

	response += "HTTP/1.1 201 Created\r\n";
	response += "Content-Type: " + mimeType + "\r\n";

	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	response += content;

	return (response);
}

/*response to a DELETE request*/
std::string Response::ft_delete()
{
	Server::log("Server's receive a DELETE request.", 1);

	if (access(getFilePath().c_str(), F_OK) != 0)
	{
		std::string response = "HTTP/1.1 404 Not Found\r\n";
		response += "Content-Type: text/html\r\n";
		std::string content = "File not found";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
		response += "File not found";

		return (response);
	}

	if (access(getFilePath().c_str(), W_OK) != 0)
	{
		std::string response = "HTTP/1.1 403 Forbidden\r\n";
		response += "Content-Type: text/html\r\n";
		std::string content = "Permission denied";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
		response += content;

		return (response);
	}

	if (unlink(getFilePath().c_str()) == 0)
	{
		std::string response = "HTTP/1.1 204 No Content\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";

		return (response);
	}
	else
	{
		Server::log("Server failed to respond at the DELETE request.", 2);
		std::string response = "HTTP/1.1 500 Internal Server Error\r\n";
		response += "Content-Type: text/html\r\n";
		std::string content = "Internal server error";
		std::ostringstream oss;
		oss << content.size();
		response += "Content-Length: " + oss.str() + "\r\n";
		response += "Connection: close\r\n";
		response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
		response += content;

		return (response);
	}
}

/*response to a bad request*/
std::string Response::ft_badRequest()
{
	char *tmp = getcwd(NULL, 0);
	std::string root(tmp, strlen(tmp));
	free(tmp);
	std::string content;
	if (!this->_errorPages.empty() && (this->_errorPages.find(404) != this->_errorPages.end()))
	{
		content = readFileContent(this->_root + this->_errorPages.find(404)->second);
	}
	else
	{
		content = readFileContent(root + "/www/error_pages/404.html");
	}

	std::string response = "HTTP/1.1 404 Not Found\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	response += content;

	return (response);
}

std::string Response::ft_forbidden()
{
	char *tmp = getcwd(NULL, 0);
	std::string root(tmp, strlen(tmp));
	free(tmp);
	std::string content = readFileContent(root + "/www/error_pages/403.html");

	std::string response = "HTTP/1.1 403 Bad Request\r\n";
	response += "Content-Type: text/html\r\n";
	std::ostringstream oss;
	oss << content.size();
	response += "Content-Length: " + oss.str() + "\r\n";
	response += "Connection: close\r\n";
	response += "Server: " + *this->_serverName.begin() + "\r\n\r\n";
	response += content;
	return (response);
}

std::string Response::readFileContent(std::string path)
{
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		char *tmp = getcwd(NULL, 0);
		std::string root(tmp, strlen(tmp));
		free(tmp);
		return (readFileContent(root + "/www/error_pages/404.html"));
	}
	std::ostringstream oss;
	oss << file.rdbuf();
	return (oss.str());
}

std::string Response::getMimeType()
{
	if (this->_filePath == this->_root + "/")
		return ("text/html");

	size_t ext = this->_filePath.rfind(".");
	if (ext != std::string::npos)
	{
		std::string extension = this->_filePath.substr(ext);
		if (this->_mimePath.find(extension) != this->_mimePath.end())
		{
			return (this->_mimePath[extension]);
		}
	}
	Server::log("Extension of the files was not recognize.", 1);
	return ("application/octet-stream");
}

std::map<std::string, std::string> Response::createMimePath()
{
	std::map<std::string, std::string> mimePath;

	mimePath[".sh"] = "text/html";
	mimePath[".py"] = "text/html";
	mimePath[".html"] = "text/html";
	mimePath[".css"] = "text/css";
	mimePath[".js"] = "application/javascript";
	mimePath[".json"] = "application/json";
	mimePath[".jpg"] = "image/jpeg";
	mimePath[".jpeg"] = "image/jpeg";
	mimePath[".png"] = "image/png";
	mimePath[".gif"] = "image/gif";
	mimePath[".bmp"] = "image/bmp";
	mimePath[".ico"] = "image/x-icon";
	mimePath[".webp"] = "image/webp";
	mimePath[".svg"] = "image/svg+xml";
	mimePath[".mp4"] = "video/mp4";
	mimePath[".webm"] = "video/webm";
	mimePath[".avi"] = "video/x-msvideo";
	mimePath[".mp3"] = "audio/mpeg";
	mimePath[".pdf"] = "application/pdf";
	mimePath[".xml"] = "application/xml";
	mimePath[".ttf"] = "font/ttf";
	mimePath[".woff"] = "font/woff";
	mimePath[".woff2"] = "font/woff2";
	mimePath[".csv"] = "text/csv";
	return (mimePath);
}

/*----------------------------------------------------------------------------*/
/*                                   SETTER                                   */
/*----------------------------------------------------------------------------*/

void Response::setFilePath(std::string root, std::string fileRequested)
{
	this->_filePath = root + fileRequested;
}

/*----------------------------------------------------------------------------*/
/*                                   GETTER                                   */
/*----------------------------------------------------------------------------*/

std::string Response::getAlias() const
{
	return (this->_alias);
}

std::string Response::getRoot() const
{
	return (this->_root);
}

std::string Response::getIndex() const
{
	return (this->_index);
}

int Response::getClientMaxBodySize() const
{
	return (this->_clientMaxBodySize);
}

std::map<std::string, std::string> Response::getCgi() const
{
	return (this->_interpreterMap);
}

std::map<std::string, int> Response::getAllowedMethodsTab() const
{
	return (this->_allowed_methods);
}

int Response::getAutoIndex() const
{
	return (this->_autoIndex);
}

std::string Response::getFilePath() const
{
	return (this->_filePath);
}

std::map<int, std::string> Response::getErrorPage() const
{
	return (this->_errorPages);
}

std::map<int, std::string> Response::getRedirection() const
{
	return (this->_redirection);
}

/*----------------------------------------------------------------------------*/
/*                                  UTILS                                     */
/*----------------------------------------------------------------------------*/

std::ostream &operator<<(std::ostream &Cout, Response const &response)
{
	Cout << "alias			:" << response.getAlias() << std::endl;
	Cout << "root			:" << response.getRoot() << std::endl;
	Cout << "index			:" << response.getIndex() << std::endl;
	Cout << "Clientbody		:" << response.getClientMaxBodySize() << std::endl;
	Cout << "Autoindex		:" << response.getAutoIndex() << std::endl;
	Cout << "Cgi map		:" << std::endl;
	std::map<std::string, std::string> pathinfo = response.getCgi();
	std::map<std::string, std::string>::iterator it1 = pathinfo.begin();
	for (; it1 != pathinfo.end(); it1++)
	{
		Cout << it1->first << " | " << it1->second << std::endl;
	}
	Cout << "Allow map		: " << std::endl;
	std::map<std::string, int> allowedmet = response.getAllowedMethodsTab();
	std::map<std::string, int>::iterator it2 = allowedmet.begin();
	for (; it2 != allowedmet.end(); it2++)
	{
		Cout << it2->first << " | " << it2->second << std::endl;
	}
	Cout << "Error map		: " << std::endl;
	std::map<int, std::string> error = response.getErrorPage();
	std::map<int, std::string>::iterator it3 = error.begin();
	for (; it3 != error.end(); it3++)
	{
		Cout << it3->first << " | " << it3->second << std::endl;
	}
	return (Cout);
}
