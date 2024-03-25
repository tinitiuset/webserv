#ifndef REQUESTLIST_HPP
# define REQUESTLIST_HPP

# include <map>
# include <string>
# include <cstring>
# include "Request.hpp"
# include "../utils/defaults.hpp"
# include "../exceptions/RequestException.hpp"

class Request;

class RequestList {
	public:
		RequestList();
		RequestList(const RequestList&);
		RequestList& operator=(const RequestList&);
		virtual ~RequestList();

		Request* getRequest(const int&);
		void addRequest(const int&, Request*);
		void removeRequest(const int&);

	protected:
		std::map<int, Request *> _requests;
};

#endif
