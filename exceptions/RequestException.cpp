#include "RequestException.hpp"

RequestException::RequestException(int status) {
	_status = status;
}

RequestException::~RequestException() throw() {
}

int RequestException::status() const {
	return _status;
}