#include <iostream>
#include <string>
#include <unistd.h>

std::string extractCGIFileName(const std::string& uri) {
  
    std::string cgiFileName;
    // Buscar el primer interrogante en el nombre del archivo CGI (posición del interrogante)
    size_t questionMarkPos = uri.find('?');
    if (questionMarkPos != std::string::npos) {
        // Si hay un interrogante, eliminar la parte de la cadena después del interrogante
        cgiFileName = uri.substr(0, questionMarkPos);
    }

    return cgiFileName;
}

std::string extractQStr(std::string uri)
{
	size_t questionMarkPos = uri.find('?');
	if (questionMarkPos == std::string::npos)
		return "";

	std::string qStr = uri.substr(questionMarkPos + 1);

	return qStr;
}

int main() {
    std::string uri = "./sites/site2/cgi-bin/test.py?nombre=Juan&edad=25";
    std::string cgiFileName = extractCGIFileName(uri);
    //std::string arg0 = "python3";
    std::string interpret = "/usr/local/bin/python3";

    std::string QS = extractQStr(uri);
    std::cout << "QS: " << QS << std::endl;


    if (!cgiFileName.empty()) {
        std::cout << "Nombre del archivo CGI: " << cgiFileName << std::endl;
    } else {
        std::cerr << "Error al extraer el nombre del archivo CGI." << std::endl;
    }

    char* scriptArgs[] = {(char *)interpret.c_str(), (char *)cgiFileName.c_str(), NULL};
    //execute with excve
    execve(interpret.c_str(), const_cast<char* const*>(scriptArgs), NULL);

    return 0;
}