#include "Location.hpp"

Location::Location(){
	_path = "";
	return;
}

Location::~Location(){
	return;
}

std::string Location::getPath() const{
	return (_path);
}