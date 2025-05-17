#ifndef POLYBIUS_CIPHER_H
#define POLYBIUS_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"

class polybius_cipher : public basic_cripto {
private:
    std::string key;
    std::vector<std::vector<char>> square;
    int squareSize; // Размер квадрата

    void generateSquare() {
        // Создаем алфавит с русскими и английскими буквами и спецсимволами
        std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                              "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                              "0123456789 ,.;:!?-_\"'()[]{}";
        
        // Вычисляем размер квадрата (округляем в большую сторону)
        squareSize = std::ceil(std::sqrt(alphabet.size()));
        square = std::vector<std::vector<char>>(squareSize, std::vector<char>(squareSize, ' '));
        
        // Сначала добавляем символы из ключа
        std::string unique = key;
        
        // Затем добавляем остальные символы алфавита
        for (char c : alphabet) {
            if (unique.find(c) == std::string::npos) {
                unique += c;
            }
        }
        
        // Заполняем квадрат
        int k = 0;
        for (int i = 0; i < squareSize && k < unique.length(); i++) {
            for (int j = 0; j < squareSize && k < unique.length(); j++) {
                square[i][j] = unique[k++];
            }
        }
    }

    std::pair<int, int> findPosition(char c) {
        for (int i = 0; i < squareSize; i++) {
            for (int j = 0; j < squareSize; j++) {
                if (square[i][j] == c) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; // Символ не найден
    }

public:
    polybius_cipher(const std::string& key_value = "") {
        if (key_value.empty()) {
            // Если ключ не задан, используем случайное слово
            std::vector<std::string> defaultKeys = {"POLYBIUS", "SQUARE", "MATRIX", "CIPHER", "CRYPTOGRAPHY"};
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, defaultKeys.size() - 1);
            key = defaultKeys[dis(gen)];
        } else {
            key = key_value;
        }
        
        generateSquare();
    }

    std::string encode(const std::string message) override {
        std::string encrypted;
        std::string keyOutput = "key=" + key;
        
        for (char c : message) {
            auto pos = findPosition(c);
            if (pos.first != -1 && pos.second != -1) {
                // Шифруем как пару координат: строка+1, столбец+1
                encrypted += std::to_string(pos.first + 1);
                encrypted += std::to_string(pos.second + 1);
                encrypted += " "; // Разделяем координаты пробелом для удобства
            } else {
                // Если символа нет в квадрате, добавляем его как есть
                encrypted += c;
            }
        }
        
        return encrypted + "\nKEY:" + keyOutput;
    }

    std::string decode(const std::string encryptedMessage) override {
        // Разделяем сообщение и ключ
        size_t keyPos = encryptedMessage.find("\nKEY:");
        std::string message = encryptedMessage;
        
        if (keyPos != std::string::npos) {
            message = encryptedMessage.substr(0, keyPos);
            std::string keyPart = encryptedMessage.substr(keyPos + 5);
            
            // Парсим ключ
            size_t keyValuePos = keyPart.find("key=");
            if (keyValuePos != std::string::npos) {
                key = keyPart.substr(keyValuePos + 4);
                generateSquare(); // Перегенерируем квадрат с новым ключом
            }
        }
        
        std::string decrypted;
        std::string keyOutput = "key=" + key;
        
        // Разбираем зашифрованное сообщение, которое должно содержать пары чисел
        std::string currentCoord;
        for (size_t i = 0; i < message.length(); i++) {
            if (isdigit(message[i])) {
                currentCoord += message[i];
                
                // Если накопили 2 цифры - это координаты в квадрате
                if (currentCoord.length() == 2) {
                    int row = currentCoord[0] - '1'; // -1 т.к. индексы с 0
                    int col = currentCoord[1] - '1';
                    
                    if (row >= 0 && row < squareSize && col >= 0 && col < squareSize) {
                        decrypted += square[row][col];
                    }
                    
                    currentCoord.clear();
                }
            } else if (message[i] != ' ') {
                // Если это не цифра и не пробел, добавляем как есть
                if (!currentCoord.empty()) {
                    // Если у нас есть частичная координата, добавляем ее как текст
                    decrypted += currentCoord;
                    currentCoord.clear();
                }
                decrypted += message[i];
            } else {
                // Если это пробел, заканчиваем текущую координату
                if (!currentCoord.empty()) {
                    if (currentCoord.length() == 2) {
                        int row = currentCoord[0] - '1';
                        int col = currentCoord[1] - '1';
                        
                        if (row >= 0 && row < squareSize && col >= 0 && col < squareSize) {
                            decrypted += square[row][col];
                        }
                    } else {
                        // Если координата неполная, добавляем как текст
                        decrypted += currentCoord;
                    }
                    currentCoord.clear();
                }
            }
        }
        
        // Проверяем, остались ли необработанные координаты
        if (!currentCoord.empty()) {
            if (currentCoord.length() == 2) {
                int row = currentCoord[0] - '1';
                int col = currentCoord[1] - '1';
                
                if (row >= 0 && row < squareSize && col >= 0 && col < squareSize) {
                    decrypted += square[row][col];
                }
            } else {
                // Если координата неполная, добавляем как текст
                decrypted += currentCoord;
            }
        }
        
        return decrypted + "\nKEY:" + keyOutput;
    }
};

#endif // POLYBIUS_CIPHER_H
