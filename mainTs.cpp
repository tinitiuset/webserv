#include <iostream>
#include <string>
#include "Utils.hpp"

int main()
{
    std::vector<std::string> vec;
    vec.push_back("apple");
    vec.push_back("banana");
    vec.push_back("orange");
    vec.push_back("grape");


    // String que quieres encontrar
    std::string target = "9";

    // Buscar el string en el vector y obtener el índice
    int index = Utils::findStrInVector(vec, target);

    // Verificar si se encontró el string
    if (index != -1) {
        std::cout << "Se encontró '" << target << "' en el índice " << index << " del vector." << std::endl;
    } else {
        std::cout << "No se encontró '" << target << "' en el vector." << std::endl;
    }


    return 0;
}
