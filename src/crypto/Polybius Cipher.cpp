#ifndef POLYBIUS_CIPHER_H
#define POLYBIUS_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"
#include "key_storage.h"

class polybius_cipher : public basic_cripto {
    private:
        std::string full_alphabet;
        std::vector<std::vector<char>> square;
        std::string keyword;
        int square_size; // Размер квадрата
        
        void generateSquare() {
            // Определяем полный алфавит
            full_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                           "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                           "0123456789 ,.;:!?-_\"'()[]{}";
            
            // Вычисляем размер квадрата (ближайший квадрат, который вместит весь алфавит)
            square_size = std::ceil(std::sqrt(full_alphabet.size()));
            
            // Создаем квадратную матрицу нужного размера
            square = std::vector<std::vector<char>>(square_size, std::vector<char>(square_size, ' '));
            
            // Формируем алфавит для квадрата, начиная с символов в ключе
            std::string alphabet = keyword;
            
            // Добавляем оставшиеся символы из полного алфавита
            for (char c : full_alphabet) {
                alphabet += c;
            }
            
            // Удаляем дубликаты
            std::string unique;
            for (char c : alphabet) {
                if (unique.find(c) == std::string::npos) {
                    unique += c;
                }
            }
            
            // Заполняем квадрат
            int k = 0;
            for (int i = 0; i < square_size && k < unique.size(); i++) {
                for (int j = 0; j < square_size && k < unique.size(); j++) {
                    square[i][j] = unique[k++];
                }
            }
            
            // Если остались пустые места в квадрате, заполняем их специальными символами
            if (k < square_size * square_size) {
                char fill_char = '+'; // Используем символ, которого нет в алфавите
                for (int i = 0; i < square_size; i++) {
                    for (int j = 0; j < square_size; j++) {
                        if (square[i][j] == ' ') {
                            square[i][j] = fill_char++;
                        }
                    }
                }
            }
        }
        
    public:
        // Получение текущего ключа в виде строки
        std::string get_key() const {
            return "keyword=" + keyword;
        }
        
        // Установка ключа из строки
        bool set_key(const std::string& key_str) {
            size_t keyPos = key_str.find("keyword=");
            if (keyPos != std::string::npos) {
                keyword = key_str.substr(keyPos + 8);
                generateSquare();
                return true;
            }
            return false;
        }
        
        polybius_cipher(const std::string& key = "") {
            if (key.empty()) {
                // Если ключ не задан, используем случайное слово
                std::vector<std::string> defaultKeys = {"POLYBIUS", "SQUARE", "CIPHER", "MATRIX", "CRYPTO"};
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, defaultKeys.size() - 1);
                keyword = defaultKeys[dis(gen)];
            } else {
                keyword = key;
            }
            
            generateSquare();
            
            // Сохраняем ключ в хранилище
            KeyStorage::getInstance()->setPolybiusKey(get_key());
        }
        
        std::string encode(const std::string message) override {
            std::string encrypted;
            
            for (char c : message) {
                bool found = false;
                
                // Находим координаты в квадрате
                for (int i = 0; i < square_size && !found; i++) {
                    for (int j = 0; j < square_size && !found; j++) {
                        if (square[i][j] == c) {
                                                       // Используем двузначные числа для каждой координаты
                            encrypted += std::to_string(i + 1).length() == 1 ? "0" + std::to_string(i + 1) : std::to_string(i + 1);
                            encrypted += std::to_string(j + 1).length() == 1 ? "0" + std::to_string(j + 1) : std::to_string(j + 1);
                            encrypted += " ";
                            found = true;
                        }
                    }
                }
                
                // Если символ не найден в квадрате, пропускаем его
                if (!found) {
                    encrypted += "?? ";
                }
            }
            
            // Сохраняем ключ в хранилище
            KeyStorage::getInstance()->setPolybiusKey(get_key());
            
            return encrypted;
        }
        
        std::string decode(const std::string message) override {
            // Проверяем наличие ключа в хранилище
            std::string stored_key = KeyStorage::getInstance()->getPolybiusKey();
            if (stored_key.empty()) {
                throw std::runtime_error("Ошибка: Отсутствует ключ для шифра Полибия. Расшифровка невозможна.");
            }
            
            // Устанавливаем ключ из хранилища
            if (!set_key(stored_key)) {
                throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Полибия.");
            }
            
            std::string decrypted;
            std::string current_coords;
            
            for (char c : message) {
                if (isdigit(c)) {
                    current_coords += c;
                    
                    // Когда накопили 4 цифры (две координаты по 2 цифры каждая)
                    if (current_coords.length() == 4) {
                        int row = std::stoi(current_coords.substr(0, 2)) - 1;
                        int col = std::stoi(current_coords.substr(2, 2)) - 1;
                        
                        if (row >= 0 && row < square_size && col >= 0 && col < square_size) {
                            decrypted += square[row][col];
                        } else {
                            decrypted += '?'; // Если координаты вне допустимого диапазона
                        }
                        
                        current_coords.clear();
                    }
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    // Игнорируем разделители
                    continue;
                } else if (c == '?') {
                    // Для неизвестных символов (парных вопросительных знаков)
                    if (current_coords == "?") {
                        decrypted += '?';
                        current_coords.clear();
                    } else {
                        current_coords += c;
                    }
                } else {
                    // Сбрасываем накопленные координаты при любых других символах
                    current_coords.clear();
                }
            }
            
            return decrypted;
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
            std::string old_keyword = keyword;
            
            // Устанавливаем новый ключ
            if (!set_key(key_str)) {
                throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Полибия.");
            }
            
            // Дешифруем сообщение
            std::string decrypted;
            std::string current_coords;
            
            for (char c : message) {
                if (isdigit(c)) {
                    current_coords += c;
                    
                    // Когда накопили 4 цифры (две координаты по 2 цифры каждая)
                    if (current_coords.length() == 4) {
                        int row = std::stoi(current_coords.substr(0, 2)) - 1;
                        int col = std::stoi(current_coords.substr(2, 2)) - 1;
                        
                        if (row >= 0 && row < square_size && col >= 0 && col < square_size) {
                            decrypted += square[row][col];
                        } else {
                            decrypted += '?'; // Если координаты вне допустимого диапазона
                        }
                        
                        current_coords.clear();
                    }
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    // Игнорируем разделители
                    continue;
                } else if (c == '?') {
                    // Для неизвестных символов (парных вопросительных знаков)
                    if (current_coords == "?") {
                        decrypted += '?';
                        current_coords.clear();
                    } else {
                        current_coords += c;
                    }
                } else {
                    // Сбрасываем накопленные координаты при любых других символах
                    current_coords.clear();
                }
            }
            
            // Сохраняем использованный ключ в хранилище
            KeyStorage::getInstance()->setPolybiusKey(key_str);
            
            return {decrypted, key_str};
        }
};

#endif // POLYBIUS_CIPHER_H
