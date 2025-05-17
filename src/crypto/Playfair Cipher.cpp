#include "basic_cripto.h"
#include <iostream>
#include <string>


class playfair : public basic_cripto{
    private:
        std::string key;
        std::string matrica[5][5];
    public:
        playfair(std::string key = "default") : key(key) {
            // Инициализация матрицы
            std::string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // J заменен на I
            std::string usedChars;
            int index = 0;

            for (char c : key) {
                if (isalpha(c) && usedChars.find(toupper(c)) == std::string::npos) {
                    matrica[index / 5][index % 5] = toupper(c);
                    usedChars += toupper(c);
                    index++;
                }
            }

            for (char c : alphabet) {
                if (usedChars.find(c) == std::string::npos) {
                    matrica[index / 5][index % 5] = c;
                    usedChars += c;
                    index++;
                }
            }
        }

        // Шифрования и дешифрование 
        std::string encode(std::string message) override;
        std::string decode(std::string message) override;
        std::string getKey() const {
            return key;
        }
        std::string getMatrica() const {
            std::string result;
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    result += matrica[i][j];
                }
            }
            return result;
        }
        
};

int main() {
    playfair cipher("KEYWORD");
    std::string message = "HELLO WORLD";
    std::string encrypted = cipher.encode(message);
    std::string decrypted = cipher.decode(encrypted);
    std::cout << "Original: " << message << std::endl;
    std::cout << "Encrypted: " << encrypted << std::endl;
    std::cout << "Decrypted: " << decrypted << std::endl;
    std::cout << "Key: " << cipher.getKey() << std::endl;
    std::cout << "Matrix: " << cipher.getMatrica() << std::endl;
}