#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <string>
# include "Response.hpp"
# include "../utils/defaults.hpp"
# include <vector>
# include "../utils/Codes.hpp"
# include "../utils/Logger.hpp"
# include "../utils/Utils.hpp"
# include "../model/Location.hpp"

static std::string status(int code);

/* class VServer{
	private:
		std::vector<Location> _locations;
}; */

class Request {
	public:

		Request();
		Request(const Request&);
		Request(const int &fd, const std::list <Location*> locations);
		Request& operator=(const Request&);
		virtual ~Request();
		void parseRequest(const int &);
		void printRequest() const;
		bool isGetRequest() const;
		bool isPostRequest() const;
		bool isDeleteRequest() const;
		int	 getPort() const;
		std::string getHost() const;
		virtual std::string handle();
		std::string redirect();
		std::string getUri() const;

	protected:
		//void searchLocation(const std::string &path, const std::list <Location*> _locations);
		std::string _method;
		std::string _uri;
		std::map<std::string, std::string> _headers;
		std::string _body;
		//Location *_location;
};

#endif