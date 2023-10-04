#include "includes/webserv.hpp"

int	Serv::filter_request(Request &req)
{
	std::string	path_info;
	std::string extension_string;
	int	cgi_fd = 0;

	if (!(req.Get().empty()))
		path_info = req.Get();
	else if (!(req.Post().empty()))
		path_info = req.Post();

	// Important: Throw error during config parsing if the cgi is set
	// to use a static file extension (.html, .jpg, etc)

	size_t	extension_start = path_info.rfind('.');
	if (extension_start != std::string::npos)
	{
		size_t query_start = path_info.rfind('?');
		if (query_start != std::string::npos)
			extension_string = path_info.substr(extension_start, query_start - extension_start);
		else
			extension_string = path_info.substr(extension_start);
		if (extension_string == serv_info.cgi_extension)
			cgi_fd = cgi_request(req, path_info, extension_string);

	}
	return (cgi_fd);
}

int	Serv::cgi_request(Request &req, std::string path_info, std::string script_extension)
{
	std::vector<std::string> meta_vars;
	char **cgi_env;
	std::string cmd_name;
	
	init_cgi_meta_vars(req, &meta_vars);
	cgi_env = create_cgi_env(meta_vars);

	int i = -1;
	while (cgi_env[++i])
	{
		std::cout << cgi_env[i] << std::endl;
	}

	if (script_extension == ".sh")
		cmd_name = "bash";
	else if (script_extension == ".py")
		cmd_name = "python3";
	else
		cmd_name = script_extension.substr(1);
	int	cgi_fd = execute_script(findcommand("/" + cmd_name), path_info, cgi_env); 

	i = -1;
	while (cgi_env[++i])
		delete[] cgi_env[i];
	delete[] cgi_env;
	return (cgi_fd);
}

int	Serv::execute_script(std::string cmd_path, std::string path_info, char **env)
{
	int		pipe_fd[2];
	pid_t	pid;
    int status = 0;

	if (pipe(pipe_fd) == -1)
		perror("pipe");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		std::string script_name = path_info.substr(path_info.rfind("/") + 1);
		path_info = serv_info.root + path_info;
		if (chdir((serv_info.root + "/" + serv_info.cgi_directory).c_str()) == -1)
			std::cout << "Error changing to cgi dir: " << serv_info.root + "/" + serv_info.cgi_directory << std::endl;
        // char buffer[1000];
		// getcwd(buffer, sizeof(buffer));
		// std::cout << buffer << std::endl;
		char *argv[3];
        argv[0] = const_cast<char *>((cmd_path).c_str());
		argv[1] = const_cast<char *>((script_name).c_str());
		argv[2] = NULL;
		std::cout << argv[1] << std::endl;
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		if (execve(cmd_path.c_str(), argv, env) == -1)
            perror("execve");
	}
	else
	{
		close(pipe_fd[1]);
		return (pipe_fd[0]);

		// char buffer[4096];
        // ssize_t bytes_read;

		// while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
		// {
		// 	// this is the output of the executed script, it should inclue a header as well
		// 	// will need to send the contents of buffer (should include a header)
		// 	// to the parseSend function, like the getResponse function does
		// }

		// // if the script has no output, we should redirect to 
		// // the same page where the script was called ("Referer" attribute)

		close(pipe_fd[0]);
		wait(&status);
	}
	return (status);
}


void Serv::init_cgi_meta_vars(Request &req, std::vector<std::string> *meta_vars)
{
	meta_vars->push_back("AUTH_TYPE=");
	meta_vars->push_back("CONTENT_TYPE=" + req.Contenttype());
	meta_vars->push_back("CONTENT_LENGTH=" + req.Contentlength());
	meta_vars->push_back("GATEWAY_INTERFACE=CGI/1.1");
	
	std::string path_info = "";
	std::string req_method = "";
	if (!(req.Get().empty()))
	{
		path_info = req.Get();
		req_method = "GET";
	}
	else if (!(req.Post().empty()))
	{	
		path_info = req.Post();
		req_method = "POST";
	}
	// else if (!(req.Delete().empty()))
	// {
	// 	path_info = req.Delete();
	// 	req_method = "DELETE";
	// }
	meta_vars->push_back("REQUEST_METHOD=" + req_method);
	meta_vars->push_back("PATH_INFO=" + path_info);
	// Confirm with the tester
	meta_vars->push_back("PATH_TRANSLATED=" + serv_info.root + path_info);
	meta_vars->push_back("QUERY_STRING=" + req.Query());
	meta_vars->push_back("REMOTE_ADDR=");
	meta_vars->push_back("DOMAIN_NAME=");
	meta_vars->push_back("REMOTE_IDENT=");
	meta_vars->push_back("REMOTE_USER=");
	meta_vars->push_back("SCRIPT_NAME=" + path_info);
	meta_vars->push_back("SERVER_NAME=" + serv_info.host);
	meta_vars->push_back("SERVER_PORT=" + serv_info.port);
	meta_vars->push_back("SERVER_PROTOCOL=HTTP/1.1");
	meta_vars->push_back("SERVER_SOFTWARE=");
}

char	**Serv::create_cgi_env(std::vector<std::string> meta_vars)
{
	char **cgi_env = (char **)malloc(sizeof(char*) * 17 + 1);
	if (!cgi_env)
	{
		std::cerr << "Error allocating memory for the CGI env" << std::endl;
		return NULL;
	}

	int i = 0;
	std::vector<std::string>::iterator it;
	for (it = meta_vars.begin(); it != meta_vars.end(); it++)
	{
		cgi_env[i++] = strdup((*it).c_str());
	}
	cgi_env[i] = NULL;
	return cgi_env;
}