/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguillot <sguillot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:56 by sguillot          #+#    #+#             */
/*   Updated: 2024/10/02 12:59:01 by sguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* -------------------------------------------------------------------------- */
/*                                    INCLUDES                                */
/* -------------------------------------------------------------------------- */

#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <list>
#include <map>
#include <ctime>
#include <set>
#include <limits>

#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <netdb.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

/* -------------------------------------------------------------------------- */
/*                                    DEFINE                                  */
/* -------------------------------------------------------------------------- */

#define MAX_EVENTS 10
#define BUFFER_SIZE 4096
#define SIGNAL 2
#define RESET "\033[0m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RED "\033[31m"
#define BOLD "\033[1m"
#define HELP_INFOS "\nUsage : ./webserv [<file.conf] [--help]\n\n" \
    "Informations required in the configuration file (token must be separated by spaces or tabulations):\n\n" \
    "server {\n" \
    "\tlisten <port number>\n" \
    "\tserver_name <server name>\n" \
    "\terror_page <error code> <error html file>\n" \
    "\tclient_max_body_size <size in M>\n" \
    "\tindex <index file path>\n" \
    "\troot <root path>\n" \
    "\tallow_methods <on/off>\n" \
    "\tlocation <option: \"=\"> </page_name>\n\t\t" \
    "| allow_methods <methods allowed>\n\t\t| auto_index <on/off>\n\t\t" \
    "| alias <str alias>\n\t\t| root <str root> \n\t\t" \
    "| client_max_body_size <int size>\n\t\t| index <str index file path>\n\t\t" \
    "| error_page <int error code> <str error file path>\n\t\t| return <int code> <str return file path>" \
    "| cgi <str .extension> <str cgi path>\n" \
    "}\n" \
    "\nOther instructions :" \
    "\n\t- You can create multiple servers with the following tokens : server { }" \
    "\n\t- Location informations must be written in the same lime separated by pipes\n"
    

extern int g_signal;

/* -------------------------------------------------------------------------- */
/*                                FORWARD DECLARATIONS                        */
/* -------------------------------------------------------------------------- */


#include "Location.hpp"
#include "Server.hpp"
#include "ServerConfiguration.hpp"
#include "Response.hpp"
#include "listeningSocket.hpp"
#include "Client.hpp"
#include "Cgi.hpp"

/* -------------------------------------------------------------------------- */
/*                                    UTILS                                   */
/* -------------------------------------------------------------------------- */

/* utils.cpp */
int     ft_strcmp(const char *s1, const char *s2);
