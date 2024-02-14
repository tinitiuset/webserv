#ifndef LOCATION_HPP
#define LOCATION_HPP

#include<iostream>

class Location{
	private:
		std::string _path;
	public:
		Location();
		~Location();
		std::string getPath() const;
};

#endif