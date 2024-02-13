#include <iostream>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <fstream>


int main()
{
	std::string uri = "/www";
	std::string root = "./cgi-bin";

	std::string result = "";
	
	
	DIR *dir;
    struct dirent *ent;

	result += "<html>\n<head>\n<title>Index of ";
	result += uri + "</title>\n";
	result += "</head>\n<body>\n<h1>Index of ";
	result += uri;
	result += "</h1>\n<ul>\n";

	result += "<li>\n<a href=\"" + uri + "/" + "..\">" + ".." + "</a>\n</li>\n";

    // Abre el directorio actual
    if ((dir = opendir(root.c_str())) != NULL)
	{
        // Lee cada entrada en el directorio
        while ((ent = readdir(dir)) != NULL) 
		{
            std::string	file(ent->d_name);
			if (file != "." && file != "..")
				result += "<li>\n<a href=\"" + uri + "/" + file + "\">" + file + "</a>\n</li>\n";
        }
        closedir(dir); // Cierra el directorio
    }
	else if
	{
        // Error al abrir el directorio
        std::cerr << "No se pudo abrir el directorio" << std::endl;
        return EXIT_FAILURE;
    }
	result += "</ul>\n</body>\n</html>\n";

	std::cout << result << std::endl;


	std::ofstream archivo("output.html");
    if (archivo.is_open()) {
        archivo << result;
        archivo.close();
        std::cout << "Se ha escrito el contenido en el archivo output.html" << std::endl;
    } else {
        std::cerr << "No se pudo abrir el archivo para escritura" << std::endl;
        return EXIT_FAILURE;
    }

	return (0);
}