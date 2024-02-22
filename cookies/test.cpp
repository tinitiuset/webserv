#include <iostream>

// Función de hash simple para cadenas de caracteres
unsigned int hashString(const char* str) {
    unsigned int hashValue = 0;

    while (*str) {
        hashValue = (hashValue << 5) + *str++;
    }

    return hashValue;
}

int main() {
    //const char* str = "Hola, mundo!";
	int i = 4;
	const char *str = std::to_string(i * 4242).c_str();

    // Utilizar la función de hash para generar un hash
    unsigned int hashValue = hashString(str);

    std::cout << "Hash de la cadena: " << hashValue << std::endl;

    return 0;
}
