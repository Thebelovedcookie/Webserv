/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmahfoud <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:25:35 by mmahfoud          #+#    #+#             */
/*   Updated: 2024/10/01 23:59:37 by mmahfoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

/*----------------------------------------------------------------------------*/
/*                               CONSTRUCTOR                                  */
/*----------------------------------------------------------------------------*/

Cgi::Cgi()
{
	this->_myEnvp = NULL;
	this->_path = NULL;
}
 
Cgi::~Cgi() {}

/*----------------------------------------------------------------------------*/
/*                              METHOD/SERVER                                 */
/*----------------------------------------------------------------------------*/

void	Cgi::setEnv(std::map<std::string, std::string> env)
{
	this->_env = env;
}


char	**Cgi::conversionEnvFunc()
{
	this->_myEnvp = (char**)malloc((this->_env.size() + 1) * sizeof(char *));

	std::map<std::string, std::string>::const_iterator it = this->_env.begin();
	for (int i = 0; it != this->_env.end(); it++, i++)
	{
		std::string tmp = it->first + "=" + it->second;
		this->_myEnvp[i] = strdup(tmp.c_str());
	}
	int i = this->_env.size();
	this->_myEnvp[i] = NULL;
	return (this->_myEnvp);
}

std::string	Cgi::executeCgi()
{
	int pipefd[2];
	if (pipe(pipefd) == -1)
		error("Error: pipe creation failed");
	int pid = fork();
	if (pid == -1)
		error("Error: fork cgi failed");
	else if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		char **argv = (char **)malloc(3 * sizeof(char *));
		argv[0] = strdup((char *)this->_executer.c_str());
		argv[1] = strdup((char *)this->_path);
		argv[2] = NULL;
		char **envp = this->conversionEnvFunc();
		if (execve(argv[0], argv, envp) == -1)
		{
			error("Error: execve cgi failed");
			freeTab(&argv);
			freeTab(&envp);
		}
	}
	close(pipefd[1]);
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	int n = 0;
	while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
		buffer[n] = '\0';
	close(pipefd[0]);
	waitpid(pid, NULL, 0);
	std::string getContent(buffer, strlen(buffer));
	return (getContent);
}

/*----------------------------------------------------------------------------*/
/*                                  UTILS                                     */
/*----------------------------------------------------------------------------*/

void	Cgi::error(std::string errorType) {
	throw(std::runtime_error(errorType));
}

void	Cgi::setPath(const char *path) {
	this->_path = path;
}

void	Cgi::setExecuter(std::string executer) {
	this->_executer = executer;
}

void Cgi::freeTab(char ***tab)
{
	int	i;

	i = 0;
	while (*tab[i])
	{
		free (*tab[i]);
		i++;
	}
	free(*tab);
}
