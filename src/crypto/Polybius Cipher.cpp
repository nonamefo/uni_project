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
                            encrypted += std::to_string
