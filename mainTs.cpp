#include <iostream>
#include <string>

int main() {
    std::string ruta = "/ruta/de/ejemplo/archivo.txt";
    //std::string ruta = "/ruta/de/ejemplo/"; // Prueba
    //std::string ruta = "/"; // Prueba con solo un "/"

    // Encontrar la posición del penúltimo "/"
    size_t pos = ruta.find_last_of("/");
    if (pos != std::string::npos && pos != 0) {
        // Buscar la posición del penúltimo "/"
        size_t penultimo_pos = ruta.find_last_of("/", pos - 1);
        if (penultimo_pos != std::string::npos) {
            // Eliminar la última parte de la cadena a partir del penúltimo "/"
            ruta.erase(penultimo_pos + 1);
        }
    }

    std::cout << "Ruta resultante: " << ruta << std::endl;

    return 0;
}

