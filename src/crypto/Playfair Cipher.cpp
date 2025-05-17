#ifndef PLAYFAIR_CIPHER_H
#define PLAYFAIR_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"

class playfair_cipher : public basic_cripto {
private:
    std::string key;
    std::vector<std::vector<char>> matrix;
    int matrixSize; // Размер матрицы

    void generateMatrix() {
        // Используем расширенную матрицу для всех символов
        // Вычисляем размер квадратной матрицы
        std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                              "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                              "0123456789 ,.;:!?-_\"'()[]{}";
        
        // Чтобы упростить, создадим матрицу такого размера, чтобы вместить все символы
        matrixSize = std::ceil(std::sqrt(alphabet.size()));
        matrix = std::vector<std::vector<char>>(matrixSize, std::vector<char>(matrixSize, ' '));
        
        // Сначала добавляем символы из ключа
        std::string unique = key;
        
        // Затем добавляем остальные символы алфавита
        for (char c : alphabet) {
            if (unique.find(c) == std::string::npos) {
                unique += c;
            }
        }
        
        // Заполняем матрицу
        int k = 0;
        for (int i = 0; i < matrixSize && k < unique.length(); i++) {
            for (int j = 0; j < matrixSize && k < unique.length(); j++) {
                matrix[i][j] = unique[k++];
            }
        }
    }

    std::pair<int, int> findPosition(char c) {
        for (int i = 0; i < matrixSize; i++) {
            for (int j = 0; j < matrixSize; j++) {
                if (matrix[i][j] == c) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; // Символ не найден
    }

public:
    playfair_cipher(const std::string& key_value = "") {
        if (key_value.empty()) {
            // Если ключ не задан, используем случайное слово
            std::vector<std::string> defaultKeys = {"PLAYFAIR", "CRYPTO", "SECURITY", "PASSWORD", "SECRET"};
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, defaultKeys.size() - 1);
            key = defaultKeys[dis(gen)];
        } else {
            key = key_value;
        }
        
        generateMatrix();
    }

    std::string encode(const std::string message) override {
        std::string encrypted;
        std::string keyOutput = "key=" + key;
        
        // Подготовка сообщения: разбиваем на пары, заменяем повторяющиеся символы
        std::string prepared;
        for (size_t i = 0; i < message.length(); i++) {
            prepared += message[i];
            if (i + 1 < message.length()) {
                if (message[i] == message[i + 1]) {
                    prepared += 'X'; // Вставляем X между повторяющимися символами
                }
            }
        }
        
        // Если длина нечетная, добавляем X в конец
        if (prepared.length() % 2 != 0) {
            prepared += 'X';
        }
        
        // Шифруем пары символов
        for (size_t i = 0; i < prepared.length(); i += 2) {
            char a = prepared[i];
            char b = prepared[i + 1];
            
            auto posA = findPosition(a);
            auto posB = findPosition(b);
            
            // Если символы не найдены в матрице, добавляем их как есть
            if (posA.first == -1 || posB.first == -1) {
                encrypted += a;
                encrypted += b;
                continue;
            }
            
            int rowA = posA.first;
            int colA = posA.second;
            int rowB = posB.first;
            int colB = posB.second;
            
            if (rowA == rowB) {
                // Если в одной строке, берем символы справа по модулю размера матрицы
                encrypted += matrix[rowA][(colA + 1) % matrixSize];
                encrypted += matrix[rowB][(colB + 1) % matrixSize];
            } else if (colA == colB) {
                // Если в одном столбце, берем символы снизу по модулю размера матрицы
                encrypted += matrix[(rowA + 1) % matrixSize][colA];
                encrypted += matrix[(rowB + 1) % matrixSize][colB];
            } else {
                // Образуют прямоугольник, берем противоположные углы
                encrypted += matrix[rowA][colB];
                encrypted += matrix[rowB][colA];
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
                generateMatrix(); // Перегенерируем матрицу с новым ключом
            }
        }
        
        std::string decrypted;
        std::string keyOutput = "key=" + key;
        
        // Дешифруем пары символов
        for (size_t i = 0; i < message.length(); i += 2) {
            if (i + 1 >= message.length()) break;
            
            char a = message[i];
            char b = message[i + 1];
            
            auto posA = findPosition(a);
            auto posB = findPosition(b);
            
            // Если символы не найдены в матрице, добавляем их как есть
            if (posA.first == -1 || posB.first == -1) {
                decrypted += a;
                decrypted += b;
                continue;
            }
            
            int rowA = posA.first;
            int colA = posA.second;
            int rowB = posB.first;
            int colB = posB.second;
            
            if (rowA == rowB) {
                // Если в одной строке, берем символы слева по модулю размера матрицы
                decrypted += matrix[rowA][(colA - 1 + matrixSize) % matrixSize];
                decrypted += matrix[rowB][(colB - 1 + matrixSize) % matrixSize];
            } else if (colA == colB) {
                // Если в одном столбце, берем символы сверху по модулю размера матрицы
                decrypted += matrix[(rowA - 1 + matrixSize) % matrixSize][colA];
                decrypted += matrix[(rowB - 1 + matrixSize) % matrixSize][colB];
            } else {
                // Образуют прямоугольник, берем противоположные углы
                decrypted += matrix[rowA][colB];
                decrypted += matrix[rowB][colA];
                  }
        }
        
        // Убираем добавленные X, если они были в конце
        if (!decrypted.empty() && decrypted.back() == 'X') {
            decrypted.pop_back();
        }
        
        // Убираем X между повторяющимися символами
        std::string final;
        for (size_t i = 0; i < decrypted.length(); i++) {
            final += decrypted[i];
            if (i + 2 < decrypted.length() && decrypted[i] == decrypted[i + 2] && decrypted[i + 1] == 'X') {
                i++; // Пропускаем X
            }
        }
        
        return final + "\nKEY:" + keyOutput;
    }
};

#endif // PLAYFAIR_CIPHER_H
