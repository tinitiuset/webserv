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
		int					_sesId;
		int					_hash;
		std::map<std::string, std::string> 	_sessionDB;
		std::vector<int>	_pair;

	public:
		Cookie();

		int		generateHash(const int &id);
		bool	isValidCookie(const std::string &request);


};


#endif