/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <mmahfoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:32:32 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/10/02 13:08:42 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class ListeningSocket;
class ServerConfiguration;
class Client;

class Server
{
	private:

		/*---------------------------------------------------------------*/
		/*                            ATTRIBUT                           */
		/*---------------------------------------------------------------*/	
		
					/*--------------SOCKET HANDLING-------------*/
		struct sockaddr										_clientAdress;
		struct sockaddr_in									_address;
		std::vector<ServerConfiguration>					tab_serv;
		std::vector<ListeningSocket*>						_listSockets;
		
					/*--------------EPOLL HANDLING--------------*/
		struct epoll_event									_event;
		struct epoll_event									_events[MAX_EVENTS];
		int													_epoll_fd;
		std::vector<uint32_t>								_connexion_fd;

					/*-------------REQUEST HANDLING-------------*/
		int													_status_code;
		char												received_line[BUFFER_SIZE];
		char												socket_buffer[BUFFER_SIZE];
		ServerConfiguration									*currentConfig;
		Location											_currentLocation;
		int													fd_config;

					/*-------------PARSING HANDLING-------------*/		
		bool												insideServerBlock;
		bool												insideParamBlock;
		bool												location_started;
		std::multimap<int, std::string>						response_tab;
		std::multimap<int, Client *>						tabClient;
		
		/*---------------------------------------------------------------*/
		/*                            METHOD                             */
		/*---------------------------------------------------------------*/

					/*-------------SERVER HANDLING-------------*/
		void												creatAllListeningSockets();
		void												acceptConnexion(int sock);
		void												set_nonblocking(int sockfd);
		void												inConnexion(ListeningSocket *list, int connexionFD);
		void												outConnexionClient(int connexionFD);
		void												outConnexionServer(int connexionFD);
		std::string											handle_client(ListeningSocket *list, int current_fd, Client *client);
		void												getServBlock(Client *client, ListeningSocket *list);
		void												getLocationBlock(Client *client);
		void												closeServer();
		int													readHead(Client *client);
		void												readBody(Client *client);

					/*-------------PARSING HANDLING-------------*/
		void												parsing_g(int argc, char **argv);
		void												check_parsing(void);
		void												check_index(ServerConfiguration server_conf);
		void												check_listen(ServerConfiguration server_conf);
		void												check_location(ServerConfiguration server_conf);
		void												check_max_body(ServerConfiguration server_conf);
		void												check_root(ServerConfiguration server_conf);
		void												check_server_name(ServerConfiguration server_conf);
		void												check_interpreter_map(ServerConfiguration server_conf);
		void												check_folder(const std::string &folder_path, const std::string &server_name);
		void												check_file(const std::string &folder_path, const std::string &file_path, const std::string &server_name);
		void 												check_error_code(int error_code, const std::string &server_name);
		void 												check_language(const std::string interpreter_language, const std::string &server_name);
		void												check_listen_duplicate(void);
		void												check_server_name_duplicate(void);
		void 												location_check_blockName(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_alias(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_root(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_maxBodySize(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_index(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_cgi(Location location_conf, ServerConfiguration server_conf);
		void 												location_check_incompabilities(Location location_conf, ServerConfiguration server_conf);
		void 												readConfigurationFile(const char *arg);
		void												ft_tokenizer(std::string line);
		void												error(std::string errorType);
		void												man(std::string man);
		void												ft_param_set_tokens(std::vector<std::string> tokens);
		void												ft_invalid_line(std::vector<std::string> tokens);
		void 												ft_set_tokens(std::vector<std::string> tokens);
		void												ft_set_auto_index(std::vector<std::string> tokens);
		void 												ft_set_server_name_param(std::vector<std::string> tokens);
		void												ft_set_client_max_body_param(std::vector<std::string> tokens);
		void												ft_set_location_param(std::vector<std::string> tokens);
		void												ft_set_listen_param(std::vector<std::string> tokens);
		void												ft_set_error_page_param(std::vector<std::string> tokens);
		void												ft_set_root_param(std::vector<std::string> tokens);
		void												ft_set_index_param(std::vector<std::string> tokens);
		void												ft_set_allowed_methods(std::vector<std::string> tokens);
		void												ft_location_pages(std::vector<std::string> tokens);
		void												ft_location_pages_dispatch(std::vector<std::string> current_param, Location &new_location);
		void												ft_checkIp(const std::string &ip_str);
		void												ft_checkPort(const std::string port_str);

	public:
		void												startingServer();
		void												serverExecution();
		static void											log(std::string error, int type);
 		static std::ofstream								*_log;

		/*---------------------------------------------------------------*/
		/*                    CONSTRUCTOR/DESTRUCTOR                     */
		/*---------------------------------------------------------------*/
		Server(int argc, char **argv);
		~Server();
};