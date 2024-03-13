#ifndef REQUESTEXCEPTION_HPP
#define REQUESTEXCEPTION_HPP

#include <exception>

class RequestException: public std::exception {

private:
	int _status;

public:
	RequestException(int status);
	~RequestException() throw();
	int status() const;
};


#endif
