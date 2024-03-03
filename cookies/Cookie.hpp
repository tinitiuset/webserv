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
#include "../utils/Utils.hpp"


class Cookie
{
	private:
		static int									_sesId;				
		static std::map<std::string, std::string> 	_sessionDB;
		static int									_hash;
		static std::string							_newKey;
		static std::string							_newValue;

	public:
		Cookie();

		static int				getSesId();
		static int				generateHash();
		static void				generateNewCookie();
		static bool 			isValidCookie(const std::map<std::string, std::string>& _headers);
		static std::string		getSetCookieValue();
};


#endif