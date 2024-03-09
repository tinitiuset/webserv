#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <map>
# include <string>

class Response {

private:
	std::string _start_line;
	std::map<std::string, std::string> _headers;
	std::string _body;

public:
	Response();
	Response(const Response&);
	Response& operator=(const Response&);
	~Response();

	std::string start_line() const;
	void set_start_line(const std::string&);

	std::map<std::string, std::string> headers() const;
	void set_headers(const std::map<std::string, std::string>&);

	std::string body() const;
	void set_body(const std::string&);

	std::string format() const;
};

#endif
