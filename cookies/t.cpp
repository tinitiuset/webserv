#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

void buscarClavesEnCookies(const std::map<std::string, std::string>& headers)
{
    //meter cada linea derl archivo "cookies.txt" en un vector eliminando /n y /r si los hay
	std::vector<std::string> cookies;
	std::ifstream file("cookies.txt");
	std::string line;
	while (std::getline(file, line))
	{
		//busca en line /n y /r y elimina
		cookies.push_back(line);
		

	}
	file.close();



	
	std::map<std::string, std::string>::const_iterator it = headers.find("Cookie");

    if (it != headers.end())
       
}

int main() {
    // Mapa para almacenar los headers y cookies
    std::map<std::string, std::string> headers;

    // Agregar algunos headers y cookies para ilustrar
    headers["Cookie"] = "webserv=127773; otro=456; webserv=1234; otro=012";
    headers["Content-Type"] = "text/html";

    buscarClavesEnCookies(headers);

    return 0;
}
