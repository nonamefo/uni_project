#ifndef VIGENERE_CIPHER_H
#define VIGENERE_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"

class vigenere_cipher : public basic_cripto {
private:
    std::string key;
    std::string alphabet;

    // Получение индекса символа в алфавите
    int getIndex(char c) {
        auto it = std::find(alphabet.begin(), alphabet.end(), c);
        if (it != alphabet.end()) {
            return std::distance(alphabet.begin(), it);
        }
        return -1;
    }

public:
    vigenere_cipher(const std::string& key_value = "") {
        // Алфавит включает английские и русские буквы, а также основные символы
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                   "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                   "0123456789 ,.;:!?-_\"'()[]{}";
        
        if (key_value.empty()) {
            // Если ключ не задан, используем случайное слово
            std::vector<std::string> defaultKeys = {"VIGENERE", "CIPHER", "SECRET", "PASSWORD", "CRYPTO"};
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, defaultKeys.size() - 1);
            key = defaultKeys[dis(gen)];
        } else {
            key = key_value;
        }
    }

    std::string encode(const std::string message) override {
        std::string encrypted;
        std::string keyOutput = "key=" + key;
        
        size_t keyIndex = 0;
        for (char c : message) {
            int charIndex = getIndex(c);
            if (charIndex != -1) {
                // Получаем индекс текущего символа ключа
                int keyCharIndex = getIndex(key[keyIndex % key.length()]);
                if (keyCharIndex == -1) {
                    keyCharIndex = 0; // Если символ ключа не в алфавите, используем 0
                }
                
                // Шифруем по формуле шифра Виженера: (charIndex + keyCharIndex) % alphabet.size()
                int encryptedIndex = (charIndex + keyCharIndex) % alphabet.size();
                encrypted += alphabet[encryptedIndex];
                
                keyIndex++; // Переходим к следующему символу ключа
            } else {
                // Если символа нет в алфавите, добавляем его как есть
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
            }
        }
        
        std::string decrypted;
        std::string keyOutput = "key=" + key;
        
        size_t keyIndex = 0;
        for (char c : message) {
            int charIndex = getIndex(c);
            if (charIndex != -1) {
                // Получаем индекс текущего символа ключа
                int keyCharIndex = getIndex(key[keyIndex % key.length()]);
                if (keyCharIndex == -1) {
                    keyCharIndex = 0; // Если символ ключа не в алфавите, используем 0
                }
                
                // Дешифруем по формуле: (charIndex - keyCharIndex + alphabet.size()) % alphabet.size()
                int decryptedIndex = (charIndex - keyCharIndex + alphabet.size()) % alphabet.size();
                decrypted += alphabet[decryptedIndex];
                
                keyIndex++; // Переходим к следующему символу ключа
            } else {
                // Если символа нет в алфавите, добавляем его как есть
                decrypted += c;
            }
        }
        
        return decrypted + "\nKEY:" + keyOutput;
    }
};

#endif // VIGENERE_CIPHER_H
