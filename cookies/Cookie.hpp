/* 
	en una request si no hay cookie o no es valida, se genera una nueva cookie y se setea en la response

	metodo para comprobar si hay cookie y es valida.
	metodo para generar una nueva cookie
	metodo para guardar en un mapa la cookie
	metodo para generar la respuesta con set-cookie

*/
#ifndef COOKIE_HPP
#define COOKIE_HPP


#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream> 
#include "../utils/Utils.hpp"


class Cookie
{
	private:
		
	public:
		Cookie();


		static	void 					removeNlCr(std::string &line);
		static std::vector<std::string> extractCookieValues(const std::string &input);
		
		static std::string				getSetCookieValue();
		static bool 					isValidCookie(const std::map<std::string, std::string>& _headers);

};


#endif