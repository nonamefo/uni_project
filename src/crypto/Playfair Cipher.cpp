#ifndef PLAYFAIR_CIPHER_H
#define PLAYFAIR_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"
#include "key_storage.h"

class playfair_cipher : public basic_cripto {
    private:
        std::string key;
        std::vector<std::vector<char>> matrix;
        std::string full_alphabet;
        int matrix_size; // Размер квадратной матрицы
        #ifndef PLAYFAIR_CIPHER_H
#define PLAYFAIR_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"
#include "key_storage.h"

class playfair_cipher : public basic_cripto {
    private:
        std::string key;
        std::vector<std::vector<char>> matrix;
        std::string full_alphabet;
        int matrix_size; // Размер квадратной матрицы
        
        void generateMatrix() {
            // Определяем полный алфавит
            full_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                           "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                           "0123456789 ,.;:!?-_\"'()[]{}";
            
            // Вычисляем размер матрицы (ближайший квадрат, который вместит весь алфавит)
            matrix_size = std::ceil(std::sqrt(full_alphabet.size()));
            
            // Создаем квадратную матрицу нужного размера
            matrix = std::vector<std::vector<char>>(matrix_size, std::vector<char>(matrix_size, ' '));
            
            // Формируем алфавит для матрицы, начиная с символов в ключе
            std::string alphabet = key;
            
            // Добавляем оставшиеся символы из полного алфавита
            for (char c : full_alphabet) {
                alphabet += c;
            }
            
            // Удаляем дубликаты символов
            std::string unique;
            for (char c : alphabet) {
                if (unique.find(c) == std::string::npos) {
                    unique += c;
                }
            }
            
            // Заполняем матрицу
            int k = 0;
            for (int i = 0; i < matrix_size && k < unique.size(); i++) {
                for (int j = 0; j < matrix_size && k < unique.size(); j++) {
                    matrix[i][j] = unique[k++];
                }
            }
            
            // Если остались пустые места в матрице, заполняем их специальными символами
            if (k < matrix_size * matrix_size) {
                char fill_char = '+'; // Используем символ, которого нет в алфавите
                for (int i = 0; i < matrix_size; i++) {
                    for (int j = 0; j < matrix_size; j++) {
                        if (matrix[i][j] == ' ') {
                            matrix[i][j] = fill_char++;
                        }
                    }
                }
            }
        }
        
        std::pair<int, int> findPosition(char c) {
            for (int i = 0; i < matrix_size; i++) {
                for (int j = 0; j < matrix_size; j++) {
                    if (matrix[i][j] == c) {
                        return {i, j};
                    }
                }
            }
            // Если символ не найден, возвращаем первую позицию (для замены неизвестных символов)
            return {0, 0};
        }

    public:
        // Получение текущего ключа в виде строки
        std::string get_key() const {
            return "key=" + key;
        }
        
        // Установка ключа из строки
        bool set_key(const std::string& key_str) {
            size_t keyPos = key_str.find("key=");
            if (keyPos != std::string::npos) {
                key = key_str.substr(keyPos + 4);
                generateMatrix();
                return true;
            }
            return false;
        }
        
        playfair_cipher(const std::string& key_value = "") {
            if (key_value.empty()) {
                // Если ключ не задан, используем случайное слово
                std::vector<std::string> defaultKeys = {"MONARCHY", "PLAYFAIR", "KEYWORD", "SECURITY", "CRYPTOGRAPHY"};
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, defaultKeys.size() - 1);
                key = defaultKeys[dis(gen)];
            } else {
                key = key_value;
            }
            
            generateMatrix();
            
            // Сохраняем ключ в хранилище
            KeyStorage::getInstance()->setPlayfairKey(get_key());
        }
        
        std::string encode(const std::string message) override {
            // Подготовка сообщения - разбиваем на пары, разделяя одинаковые символы
            std::string prepared;
            for (size_t i = 0; i < message.length(); i++) {
                prepared += message[i];
                if (i + 1 < message.length()) {
                    if (message[i] == message[i + 1]) {
                        prepared += 'X'; // Разделитель для одинаковых символов
                    }
                }
            }
            
            // Если длина нечетная, добавляем X в конец
            if (prepared.length() % 2 != 0) {
                prepared += 'X';
            }
            
            std::string encrypted;
            for (size_t i = 0; i < prepared.length(); i += 2) {
                char a = prepared[i];
                char b = prepared[i + 1];
                
                auto posA = findPosition(a);
                auto posB = findPosition(b);
                
                int rowA = posA.first;
                int colA = posA.second;
                int rowB = posB.first;
                int colB = posB.second;
                
                if (rowA == rowB) {
                    // Если в одной строке, берем символы справа
                    encrypted += matrix[rowA][(colA + 1) % matrix_size];
                    encrypted += matrix[rowB][(colB + 1) % matrix_size];
                } else if (colA == colB) {
                    // Если в одном столбце, берем символы снизу
                    encrypted += matrix[(rowA + 1) % matrix_size][colA];
                    encrypted += matrix[(rowB + 1) % matrix_size][colB];
                } else {
                    // Формируем прямоугольник
                    encrypted += matrix[rowA][colB];
                    encrypted += matrix[rowB][colA];
                }
            }
            
            // Сохраняем ключ в хранилище
            KeyStorage::getInstance()->setPlayfairKey(get_key());
            
            return encrypted;
        }
        
        std::string decode(const std::string message) override {
            // Проверяем наличие ключа в хранилище
            std::string stored_key = KeyStorage::getInstance()->getPlayfairKey();
            if (stored_key.empty()) {
                throw std::runtime_error("Ошибка: Отсутствует ключ для шифра Плейфера. Расшифровка невозможна.");
            }
            
            // Устанавливаем ключ из хранилища
            if (!set_key(stored_key)) {
                throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Плейфера.");
            }
            
            std::string decrypted;
            
            for (size_t i = 0; i < message.length(); i += 2) {
                if (i + 1 >= message.length()) break;
                
                char a = message[i];
                char b = message[i + 1];
                
                auto posA = findPosition(a);
                auto posB = findPosition(b);
                
                int rowA = posA.first;
                int colA = posA.second;
                int rowB = posB.first;
                int colB = posB.second;
                
                if (rowA == rowB) {
                    // Если в одной строке, берем символы слева
                    decrypted += matrix[rowA][(colA + matrix_size - 1) % matrix_size];
                    decrypted += matrix[rowB][(colB + matrix_size - 1) % matrix_size];
                } else if (colA == colB) {
                    // Если в одном столбце, берем символы сверху
                    decrypted += matrix[(rowA + matrix_size - 1) % matrix_size][colA];
                    decrypted += matrix[(rowB + matrix_size - 1) % matrix_size][colB];
                } else {
                    // Формируем прямоугольник
                    decrypted += matrix[rowA][colB];
                    decrypted += matrix[rowB][colA];
                }
            }
            
            // Удаляем разделители X между одинаковыми символами
            std::string result;
            for (size_t i = 0; i < decrypted.length(); i++) {
                if (decrypted[i] != 'X' || (i > 0 && i < decrypted.length() - 1 && 
                    decrypted[i - 1] != decrypted[i + 1])) {
                    result += decrypted[i];
                }
            }
            
            // Если последний символ X, и он был добавлен для четности длины, удаляем его
            if (!result.empty() && result.back() == 'X') {
                result.pop_back();
            }
            
            return result;
        }
        
        // Метод шифрования с явным указанием ключа
        std::pair<std::string, std::string> encode_with_key(const std::string message) override {
            // Сохраняем текущий ключ перед шифрованием
            std::string current_key = get_key();
            
            // Шифруем сообщение
            std::string encrypted = encode(message);
            
            return {encrypted, current_key};
        }
        
        // Метод дешифрования с явным указанием ключа
        std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key_str) override {
            // Запоминаем текущий ключ
            std::string old_key = key;
            
            // Устанавливаем новый ключ
            if (!set_key(key_str)) {
                throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Плейфера.");
            }
            
            // Дешифруем сообщение
            std::string decrypted;
            
            for (size_t i = 0; i < message.length(); i += 2) {
                if (i + 1 >= message.length()) break;
                
                char a = message[i];
                char b = message[i + 1];
                
                auto posA = findPosition(a);
                auto posB = findPosition(b);
                
                int rowA = posA.first;
                int colA = posA.second;
                int rowB = posB.first;
                int colB = posB.second;
                
                if (rowA == rowB) {
                    decrypted += matrix[rowA][(colA + matrix_size - 1) % matrix_size];
                    decrypted += matrix[rowB][(colB + matrix_size - 1) % matrix_size];
                } else if (colA == colB) {
                    decrypted += matrix[(rowA + matrix_size - 1) % matrix_size][colA];
                    decrypted += matrix[(rowB + matrix_size - 1) % matrix_size][colB];
                } else {
                    decrypted += matrix[rowA][colB];
                    decrypted += matrix[rowB][colA];
                }
            }
            
            // Удаляем разделители X между одинаковыми символами
            std::string result;
            for (size_t i = 0; i < decrypted.length(); i++) {
                if (decrypted[i] != 'X' || (i > 0 && i < decrypted.length() - 1 && 
                    decrypted[i - 1] != decrypted[i + 1])) {
                    result += decrypted[i];
                }
            }
            
            // Если последний символ X, и он был добавлен для четности длины, удаляем его
            if (!result.empty() && result.back() == 'X') {
                result.pop_back();
            }
            
            // Сохраняем использованный ключ в хранилище
            KeyStorage::getInstance()->setPlayfairKey(key_str);
            
            return {result, key_str};
        }
};

#endif // PLAYFAIR_CIPHER_H

        void generateMatrix() {
            // Определяем полный алфавит
            full_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                           "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                           "0123456789 ,.;:!?-_\"'()[]{}";
            
            // Вычисляем размер матрицы (ближайший квадрат, который вместит весь алфавит)
            matrix_size = std::ceil(std::sqrt(full_alphabet.size()));
            
            // Создаем квадратную матрицу нужного размера
            matrix = std::vector<std::vector<char>>(matrix_size, std::vector<char>(matrix_size, ' '));
            
            // Формируем алфавит для матрицы, начиная с символов в ключе
            std::string alphabet = key;
            
            // Добавляем оставшиеся символы из полного алфавита
            for (char c : full_alphabet) {
                alphabet += c;
            }
            
            // Удаляем дубликаты символов
            std::string unique;
            for (char c : alphabet) {
                if (unique.find(c) == std::string::npos) {
                    unique += c;
                }
            }
            
            // Заполняем матрицу
            int k = 0;
            for (int i = 0; i < matrix_size && k < unique.size(); i++) {
                for (int j = 0; j < matrix_size && k < unique.size(); j++) {
                    matrix[i][j] = unique[k++];
                }
            }
            
            // Если остались пустые места в матрице, заполняем их специальными символами
            if (k < matrix_size * matrix_size) {
                char fill_char = '+'; // Используем символ, которого нет в алфавите
                for (int i = 0; i < matrix_size; i++) {
                    for (int j = 0; j < matrix_size; j++) {
                        if (matrix[i][j] == ' ') {
                            matrix[i][j] = fill_char++;
                        }
                    }
                }
            }
        }
        
        std::pair<int, int> findPosition(char c) {
            for (int i = 0; i < matrix_size; i++) {
                for (int j = 0; j < matrix_size; j++) {
                    if (matrix[i][j] == c) {
                        return {i, j};
                    }
                }
            }
            // Если символ не найден, возвращаем первую позицию (для замены неизвестных символов)
            return {0, 0};
        }

    public:
        // Получение текущего ключа в виде строки
        std::string get_key() const {
            return "key=" + key;
        }
        
        // Установка ключа из строки
        bool set_key(const std::string& key_str) {
            size_t keyPos = key_str.find("key=");
            if (keyPos != std::string::npos) {
                key = key_str.substr(keyPos + 4);
                generateMatrix();
                return true;
            }
            return false;
        }
        
        playfair_cipher(const std::string& key_value = "") {
            if (key_value.empty()) {
                // Если ключ не задан, используем случайное слово
                std::vector<std::string> defaultKeys = {"MONARCHY", "PLAYFAIR", "KEYWORD", "SECURITY", "CRYPTOGRAPHY"};
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, defaultKeys.size() - 1);
                key = defaultKeys[dis(gen)];
            } else {
                key = key_value;
            }
            
            generateMatrix();
            
            // Сохраняем ключ в хранилище
            KeyStorage::getInstance()->setPlayfairKey(get_key());
        }
        
        std::string encode(const std::string message) override {
            // Подготовка сообщения - разбиваем на пары, разделяя одинаковые символы
            std::string prepared;
            for (size_t i = 0; i < message.length(); i++) {
                prepared += message[i];
                if (i + 1 < message.length()) {
                    if (message[i] == message[i + 1]) {
                        prepared += 'X'; // Разделитель для одинаковых символов
                    }
                }
            }
            
            // Если длина нечетная, добавляем X в конец
            if (prepared.length() % 2 != 0) {
                prepared += 'X';
            }
            
            std::string encrypted;
            for (size_t i = 0; i < prepared.length(); i += 2) {
                char a = prepared[i];
                char b = prepared[i + 1];
                
                auto posA = findPosition(a);
                auto posB = findPosition(b);
                
                int rowA = posA.first;
                int colA = posA.second;
                int rowB = posB.first;
                int colB = posB.second;
                
                if (rowA == rowB) {
                    // Если в одной строке, берем символы справа
                    encrypted += matrix[rowA][(colA + 1) % matrix_size];
                    encrypted += matrix[rowB][(colB + 1) % matrix_size];
                } else if (colA == colB) {
                    // Если в одном столбце, берем символы снизу
                    encrypted += matrix[(rowA + 1) % matrix_size][colA];
                    encrypted += matrix[(rowB + 1) % matrix_size][colB];
                } else {
                    // Формируем прямоугольник
                    encrypted += matrix[rowA][colB];
                    encrypted += matrix[rowB][colA];
                }
            }
            
            // Сохраняем ключ в хранилище
            KeyStorage::getInstance()->setPlayfairKey(get_key());
            
            return encrypted;
        }
        
        std::string decode(const std::string message) override {
            // Проверяем наличие ключа в хранилище
            std::string stored_key = KeyStorage::getInstance()->getPlayfairKey();
            if (stored_key.empty()) {
                throw std::runtime_error("Ошибка: Отсутствует ключ для шифра Плейфера. Расшифровка невозможна.");
            }
            
            // Устанавливаем ключ из хранилища
            if (!set_key(stored_key)) {
                throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Плейфера.");
            }
            
            std::string decrypted;
            
            for (size_t i = 0; i < message.length(); i += 2) {
                if (i + 1 >= message.length()) break;
                
                char a = message[i];
                char b = message[i + 1];
                
                auto posA = findPosition(a);
                auto posB = findPosition(b);
                
                int rowA = posA.first;
                int colA = posA.second;
                int rowB = posB.first;
                int colB = posB.second;
                
                if (rowA == rowB) {
                    // Если в одной строке, берем символы слева
                    decrypted += matrix[rowA][(colA + matrix_size - 1) % matrix_size];
                    decrypted += matrix[rowB][(colB + matrix_size - 1) % matrix_size];
                } else if (colA == colB) {
                    // Если в одном столбце, берем символы сверху
                    decrypted += matrix[(rowA + matrix_size - 1) % matrix_size][colA];
                    decrypted += matrix[(rowB + matrix_size - 1) % matrix_size][colB];
                } else {
                    // Формируем прямоугольник
                    decrypted += matrix[rowA][colB];
                    decrypted += matrix[rowB][colA];
                }
            }
            
            // Удаляем разделители X между одинаковыми символами
            std::string result;
            for (size_t i = 0; i < decrypted.length(); i++) {
                if (decrypted[i] != 'X' || (i > 0 && i < decrypted.length() - 1 && 
                    decrypted[i - 1] != decrypted[i + 1])) {
                    result += decrypted[i];
                }
            }
            
            // Если последний символ X, и он был добавлен для четности длины, удаляем его
            if (!result.empty() && result.back() == 'X') {
                result.pop_back();
            }
            
            return result;
        }
        
        // Метод шифрования с явным указанием ключа
        std::pair<std::string, std::string> encode_with_key(const std::string message) override {
            // Сохраняем текущий ключ перед шифрованием
            std::string current_key = get_key();
            
            // Шифруем сообщение
            std::string encrypted = encode(message);
            
            return {encrypted, current_key};
        }
        
        // Метод дешифрования с явным указанием ключа
        std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key_str) override {
            // Запоминаем текущий ключ
            std::string old_key = key;
            
            // Устанавливаем новый ключ
            if (!set_key(key_str)) {
                throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Плейфера.");
            }
            
            // Дешифруем сообщение
            std::string decrypted;
            
            for (size_t i = 0; i < message.length(); i += 2) {
                if (i + 1 >= message.length()) break;
                
                char a = message[i];
                char b = message[i + 1];
                
                auto posA = findPosition(a);
                auto posB = findPosition(b);
                
                int rowA = posA.first;
                int colA = posA.second;
                int rowB = posB.first;
                int colB = posB.second;
                
                if (rowA == rowB) {
                    decrypted += matrix[rowA][(colA + matrix_size - 1) % matrix_size];
                    decrypted += matrix[rowB][(colB + matrix_size - 1) % matrix_size];
                } else if (colA == colB) {
                    decrypted += matrix[(rowA + matrix_size - 1) % matrix_size][colA];
                    decrypted += matrix[(rowB + matrix_size - 1) % matrix_size][colB];
                } else {
                    decrypted += matrix[rowA][colB];
                    decrypted += matrix[rowB][colA];
                }
            }
            
            // Удаляем разделители X между одинаковыми символами
            std::string result;
            for (size_t i = 0; i < decrypted.length(); i++) {
                if (decrypted[i] != 'X' || (i > 0 && i < decrypted.length() - 1 && 
                    decrypted[i - 1] != decrypted[i + 1])) {
                    result += decrypted[i];
                }
            }
            
            // Если последний символ X, и он был добавлен для четности длины, удаляем его
            if (!result.empty() && result.back() == 'X') {
                result.pop_back();
            }
            
            // Сохраняем использованный ключ в хранилище
            KeyStorage::getInstance()->setPlayfairKey(key_str);
            
            return {result, key_str};
        }
};

#endif // PLAYFAIR_CIPHER_H
