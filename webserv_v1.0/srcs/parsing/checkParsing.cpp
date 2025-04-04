/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkParsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:45:09 by sguillot          #+#    #+#             */
/*   Updated: 2024/10/02 11:53:31 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/* Check utils */

void Server::check_folder(const std::string &folder_path, const std::string &server_name)
{
	struct stat info;
	char absolute_path[PATH_MAX];
	char current_dir[PATH_MAX];

	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		error("Error: Cannot get current working directory: " + std::string(strerror(errno)) +
			  " (" + folder_path + ") / server name -> " + server_name);
	}

	if (chdir(folder_path.c_str()) != 0)
	{
		error("Error: Cannot access directory: " + std::string(strerror(errno)) +
			  " (" + folder_path + ") / server name -> " + server_name);
	}

	if (getcwd(absolute_path, sizeof(absolute_path)) == NULL)
	{
		error("Error: Cannot get absolute path: " + std::string(strerror(errno)) +
			  " (" + folder_path + ") / server name -> " + server_name);
	}

	if (chdir(current_dir) != 0)
	{
		error("Error: Cannot restore original working directory: " + std::string(strerror(errno)) +
			  " (" + folder_path + ") / server name -> " + server_name);
	}

	if (stat(absolute_path, &info) != 0)
	{
		error("Error: Cannot access directory: " + std::string(strerror(errno)) +
			  " (" + std::string(absolute_path) + ") / server name -> " + server_name);
	}

	if (S_ISDIR(info.st_mode))
	{
		if (!(info.st_mode & S_IRUSR) || !(info.st_mode & S_IXUSR))
		{
			error("Error: Insufficient permissions on directory: (" + std::string(absolute_path) +
				  ") / server name -> " + server_name);
		}
	}
	else
	{
		error("Error: The path is not a directory: (" + std::string(absolute_path) + ") / server name -> " + server_name);
	}
}

void Server::check_file(const std::string &folder_path, const std::string &file_path, const std::string &server_name)
{
	struct stat info;

	std::string full_path;

	if (folder_path.length() > 0)
		full_path = folder_path + "/" + file_path;
	else
		full_path = file_path;

	if (stat(full_path.c_str(), &info) != 0)
	{
		error("Error: Cannot access error page file: " + std::string(strerror(errno)) +
			  " (" + full_path + ") / server name -> " + server_name);
	}

	if (!S_ISREG(info.st_mode))
	{
		error("Error: The error page path is not a regular file: (" + full_path + ") / server name -> " + server_name);
	}

	if (!(info.st_mode & S_IRUSR))
	{
		error("Error: Insufficient permissions to read the error page file: (" + full_path + ") / server name -> " + server_name);
	}
}

void Server::check_error_code(int error_code, const std::string &server_name)
{
	int valid_error_codes_array[] = {400, 401, 403, 404, 405, 408, 410, 413, 414, 429,
									 500, 501, 502, 503, 504, 505};

	std::set<int> valid_error_codes(valid_error_codes_array, valid_error_codes_array + sizeof(valid_error_codes_array) / sizeof(int));

	if (valid_error_codes.find(error_code) == valid_error_codes.end())
	{
		std::stringstream ss;
		ss << "Error: Invalid error code: " << error_code << " / server name -> " << server_name;
		error(ss.str());
	}
}

/* Check servers configurations */

void Server::check_index(ServerConfiguration server_conf)
{
	std::string root_path = server_conf.getRoot();
	std::string index = server_conf.getIndex();
	std::string index_path = root_path + "/" + index;

	if (!index.empty())
	{
		std::ifstream file(index_path.c_str());
		if (!file.good())
		{
			error("Error : Index file does not exist: " + index_path + " / server name -> " + server_conf.getServerName()[0]);
		}
	}
}

void Server::check_listen(ServerConfiguration server_conf)
{
	(void)server_conf;
	// Already checked in parsing
}

void Server::check_max_body(ServerConfiguration server_conf)
{
	long maxBodySize = server_conf.getClientMaxBodySize();
	
	if (maxBodySize < 0)
	{
		error("Error: Invalid max body size. Must be a positive value. / server name -> " + server_conf.getServerName()[0]);
	}
}

void Server::check_root(ServerConfiguration server_conf)
{
	std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";

	if (!server_conf.getRoot().empty())
	{
		check_folder(server_conf.getRoot(), serverName);
	}
}

void Server::check_server_name(ServerConfiguration server_conf)
{
	std::vector<std::string> serverName_vector = server_conf.getServerName();
	std::vector<std::string>::iterator serverName_it = serverName_vector.begin();
	for (; serverName_it != serverName_vector.end(); serverName_it++)
	{
		std::string serverName = *serverName_it;

		if (serverName.empty())
		{
			return;
		}

		if (serverName.length() > 253)
		{
			error("Error: Server name exceeds maximum length (253 characters allowed). / server name -> " + server_conf.getServerName()[0]);
		}

		for (size_t i = 0; i < serverName.length(); i++)
		{
			char c = serverName[i];
			if (!isalnum(c) && c != '-' && c != '.')
			{
				error("Error : Server name contains invalid characters. It must contains alphanumeric characters and '-' / '.' only. / server name -> " + server_conf.getServerName()[0]);
			}
		}

		if (serverName[0] == '-' || serverName[serverName.length() - 1] == '-')
		{
			error("Error : Server name can not start or end with '-'. / server name -> " + server_conf.getServerName()[0]);
		}

		std::stringstream ss(serverName);
		std::string label;
		while (std::getline(ss, label, '.'))
		{
			if (label.length() > 63)
			{
				error("Error: Each label in the server name must not exceed 63 characters. / server name -> " + server_conf.getServerName()[0]);
			}
		}
	}
}

void Server::check_language(const std::string interpreter_language, const std::string &server_name)
{
	const char *valid_languages_array[] = {".py", ".c", ".php", ".pl", ".rs", ".sh", ".cpp"};

	std::set<std::string> valid_languages(valid_languages_array, valid_languages_array + (sizeof(valid_languages_array) / sizeof(valid_languages_array[0])));

	if (valid_languages.find(interpreter_language) == valid_languages.end())
	{
		std::stringstream ss;
		ss << "Error: Non accepted programming language for CGI: " << interpreter_language << " / server name -> " << server_name;
		error(ss.str());
	}
}

void Server::check_interpreter_map(ServerConfiguration server_conf)
{
	std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";

	if (!server_conf.getInterpreterMap().empty())
	{
		std::map<std::string, std::string> interpreter_tab = server_conf.getInterpreterMap();
		std::map<std::string, std::string>::iterator interpreter_it = interpreter_tab.begin();

		for (; interpreter_it != interpreter_tab.end(); interpreter_it++)
		{
			const std::string interpreter_language = interpreter_it->first;
			const std::string &interpreter_path = interpreter_it->second;

			check_language(interpreter_language, serverName);
			check_file("", interpreter_path, serverName);
		}
	}
}

/* Check location / */

void Server::location_check_blockName(Location location_conf, ServerConfiguration server_conf)
{
	(void)server_conf;
	std::string blockName = location_conf.getBlockName();

	if (blockName.empty())
	{
		return;
	}

	if (blockName.length() > 253)
	{
		error("Error: Block name exceeds maximum length (253 characters allowed). / server name -> " + server_conf.getServerName()[0]);
	}

	for (size_t i = 0; i < blockName.length(); i++)
	{
		char c = blockName[i];
		if (!isalnum(c) && c != '-' && c != '.' && c != '/')
		{
			error("Error : Block name contains invalid characters. It must contains alphanumeric characters and '-' / '.' only. / server name -> " + server_conf.getServerName()[0]);
		}
	}

	if (blockName[0] == '-' || blockName[blockName.length() - 1] == '-')
	{
		error("Error : Block name can not start or end with '-'. / server name -> " + server_conf.getServerName()[0]);
	}

	std::stringstream ss(blockName);
	std::string label;
	while (std::getline(ss, label, '.'))
	{
		if (label.length() > 63)
		{
			error("Error: Each label in the block name must not exceed 63 characters. / server name -> " + server_conf.getServerName()[0]);
		}
	}
}

void Server::location_check_alias(Location location_conf, ServerConfiguration server_conf)
{
	(void)location_conf;
	(void)server_conf;
}

void Server::location_check_root(Location location_conf, ServerConfiguration server_conf)
{
	std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";

	if (!location_conf.getRoot().empty())
	{
		check_folder(location_conf.getRoot(), serverName);
	}
}

void Server::location_check_maxBodySize(Location location_conf, ServerConfiguration server_conf)
{
	(void)server_conf;
	long maxBodySize = location_conf.getClientMaxBodySize();

	if (maxBodySize < -1)
	{
		error("Error: Invalid max body size. Must be a positive value. / server name -> " + server_conf.getServerName()[0]);
	}
}

void Server::location_check_index(Location location_conf, ServerConfiguration server_conf)
{
	(void)server_conf;
	std::string root_path = location_conf.getRoot();
	std::string index = location_conf.getIndex();
	std::string index_path = root_path + "/" + index;

	std::ifstream file(index_path.c_str());
	if (!file.good())
	{
		error("Error : Index file does not exist: " + index_path + " / server name -> " + server_conf.getServerName()[0]);
	}
}

void Server::location_check_cgi(Location location_conf, ServerConfiguration server_conf)
{
	std::string serverName = !server_conf.getServerName().empty() ? server_conf.getServerName()[0] : "Unknown Server";

	const std::map<std::string, std::string> &cgi_map = location_conf.getCgi();

	std::map<std::string, std::string>::const_iterator cgi_it = cgi_map.begin();
	for (; cgi_it != cgi_map.end(); ++cgi_it)
	{
		const std::string cgi_language = cgi_it->first;
		const std::string &cgi_path = cgi_it->second;

		check_language(cgi_language, serverName);
		check_file("", cgi_path, serverName);
	}
}

void Server::location_check_incompabilities(Location location_conf, ServerConfiguration server_conf)
{
	/* Check Alias and Root */
	if (!(location_conf.getAlias()).empty() && !(location_conf.getRoot()).empty())
	{
		error("Error : Root and Alias are not compatible / server name -> " + server_conf.getServerName()[0]);
	}
}

void Server::check_location(ServerConfiguration server_conf)
{
	std::vector<Location> location_vector = server_conf.getLocation();
	std::vector<Location>::iterator location_it = location_vector.begin();
	for (; location_it != location_vector.end(); location_it++)
	{
		location_check_blockName(*location_it, server_conf);
		location_check_alias(*location_it, server_conf);
		location_check_root(*location_it, server_conf);
		location_check_maxBodySize(*location_it, server_conf);
		location_check_index(*location_it, server_conf);
		location_check_cgi(*location_it, server_conf);
		location_check_incompabilities(*location_it, server_conf);
	}
}

/* Checking duplicates */

void Server::check_server_name_duplicate()
{
	std::vector<std::string> unique_server_names;

	std::vector<ServerConfiguration>::iterator it = this->tab_serv.begin();
	for (; it != this->tab_serv.end(); it++)
	{
		const std::vector<std::string> &server_names = it->getServerName();

		std::vector<std::string>::const_iterator server_name_it = server_names.begin();
		for (; server_name_it != server_names.end(); server_name_it++)
		{
			if (std::find(unique_server_names.begin(), unique_server_names.end(), *server_name_it) != unique_server_names.end())
			{
				error("Error: Multiple servers have the same name: " + *server_name_it);
			}
			else
			{
				unique_server_names.push_back(*server_name_it);
			}
		}
	}
}

void Server::check_listen_duplicate()
{
	std::vector<std::string> unique_listen;

	std::vector<ServerConfiguration>::iterator it = this->tab_serv.begin();
	for (; it != this->tab_serv.end(); it++)
	{
		const std::multimap<std::string, std::string> &port_list = it->getPortList();

		std::multimap<std::string, std::string>::const_iterator listen_it = port_list.begin();
		for (; listen_it != port_list.end(); listen_it++)
		{
			std::string concat_listen = listen_it->first + listen_it->second;
			if (std::find(unique_listen.begin(), unique_listen.end(), concat_listen) != unique_listen.end())
			{
				error("Error: Multiple servers have the listen parameter: " + concat_listen);
			}
			else
			{
				unique_listen.push_back(concat_listen);
			}
		}
	}
}

/* Main checking function */

void Server::check_parsing()
{
	std::vector<ServerConfiguration> current_tab_serv = this->tab_serv;
	std::vector<ServerConfiguration>::iterator iterator_tab_serv = current_tab_serv.begin();
	for (; iterator_tab_serv != current_tab_serv.end(); iterator_tab_serv++)
	{
		check_server_name(*iterator_tab_serv);
		check_root(*iterator_tab_serv);
		check_index(*iterator_tab_serv);
		check_listen(*iterator_tab_serv);
		check_max_body(*iterator_tab_serv);
		check_interpreter_map(*iterator_tab_serv);
		check_location(*iterator_tab_serv);
	}
	check_server_name_duplicate();
	check_listen_duplicate();
}
