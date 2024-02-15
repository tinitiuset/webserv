#include "Location.hpp"

Location::Location(std::string path): _path(path){
	return;
}

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