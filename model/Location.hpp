#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <list>
#include <sstream>
#include "../utils/Utils.hpp"

class Location {
protected:
	std::string _path;

public:
	Location();
	virtual ~Location();
	virtual Location* clone() const = 0;

	std::string path() const;
	virtual void validate() const;
};

class Redirect: public Location {

private:
	int 		_code;
	std::string _redirect;

public:
	Redirect(std::string&);
	Redirect(const Redirect& other);
	~Redirect();

	Redirect* clone() const { return new Redirect(*this); }

	void validate() const;

	int code() const;
	std::string redirect() const;

};

class Index: public Location {

private:
	std::string _root;
	std::string _file;
	bool 		_autoindex;
	bool 		_cgi;
	std::list<std::string> _methods;

public:
	Index(std::string&);
	Index(const Index& other);
	~Index();

	Index* clone() const { return new Index(*this); }

	void validate() const;

	std::string root();
	std::string file();
	bool autoindex();
	bool cgi();
	std::list<std::string> methods();
	bool isMethodAllowed(std::string method);
	std::string buildRealPath(std::string &uri);

};

#endif