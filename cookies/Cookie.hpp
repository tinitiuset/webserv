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


class Cookie
{
	private:
		static int							_sesId;				
		int									_hash;
		std::string							_newKey;
		std::string							_newValue;
		std::map<std::string, std::string> 	_sessionDB;

	public:
		Cookie();

		static int		getSesId();
		int				generateHash();
		void			generateNewCookie();
		bool			isValidCookie(const std::string &request);
		std::string		getCookieResponse();
		std::string		getCookieHeader();
};


#endif