#ifndef SERVERS_HPP
# define SERVERS_HPP

#include "webserv.hpp"

class Request;
class Serv;

class Servers
{
	private :
		char **env;
		std::vector<Serv> servs;
		std::string config;
    	std::vector<pollfd> fds;
		void validate_config();
	
	public : 
		Servers() {}
		Servers(std::string file, char **environment);
		~Servers() {std::vector<Serv>().swap(servs); std::vector<pollfd>().swap(fds);}

		Request parseRecv(std::vector<pollfd> &fds, int pos);
		void	printAll() const;
		void	init();
		void	run();
		int		checkSockets(int fd);
		Serv	&getCorrectServ(Request req);
};


// Exceptions 
class InsufficientInformation : public std::exception {
	public :
		InsufficientInformation(){}
		virtual const char* what() const throw() {
			return ("Error while parsing config file\nDescription: Insufficient information on server configuration. \nYou need at least a listen port and a server address,\nas well as a '/' location with a root.");
		}
};

class DuplicateLocation : public std::exception {
	public :
		DuplicateLocation(){}
		virtual const char* what() const throw() {
			return ("Description: Duplicate location tag on server configuration.");
		}
};

class DuplicateAttribute : public std::exception {
	public :
		DuplicateAttribute(){}
		virtual const char* what() const throw() {
			return ("Description: Duplicate attribute on server configuration.");
		}
};

class InvalidLine : public std::exception {
	public :
		InvalidLine(){}
		virtual const char* what() const throw() {
			return ("Description: Invalid line while parsing configuration file.");
		}
};

class EmptyAttributeValue : public std::exception {
	public :
		EmptyAttributeValue(){}
		virtual const char* what() const throw() {
			return ("Description: Empty value on attribute line.");
		}
};

class UnenclosedAttributeLine : public std::exception {
	public :
		UnenclosedAttributeLine(){}
		virtual const char* what() const throw() {
			return ("Description: Unenclosed attribute line. Please add a ';' at the end.");
		}
};

class InvalidValues : public std::exception {
	public :
		InvalidValues(){}
		virtual const char* what() const throw() {
			return ("Description: Attribute line with unexpected number or format of values.");
		}
};

class InvalidLocationName : public std::exception {
	public :
		InvalidLocationName(){}
		virtual const char* what() const throw() {
			return "Description: location has invalid pathname.\nPlease write it in the format '/pathname',\nyou may add subpaths by repeating this syntax";
		}
};

class NotConfigFile : public std::exception {
	public :
		NotConfigFile(){}
		virtual const char* what() const throw() {
			return "Description: argument not a config file.";
		}
};


#endif