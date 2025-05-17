#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"
#include "key_storage.h"

class caesar_cipher : public basic_cripto {
private:
    int shift; // Сдвиг
    std::string alphabet; // Используемый алфавит
    
    // Получение индекса символа в алфавите
    int getIndex(char c) {
        auto it = std::find(alphabet.begin(), alphabet.end(), c);
        if (it != alphabet.end()) {
            return std::distance(alphabet.begin(), it);
        }
        return -1;
    }

public:
    caesar_cipher(int shift_value = -1) {
        // Алфавит включает английские и русские буквы, а также основные символы
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                   "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                   "0123456789 ,.;:!?-_\"'()[]{}";
        
        // Генерируем случайный сдвиг, если не предоставлен
        if (shift_value == -1) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, alphabet.size() - 1);
            shift = dis(gen);
        } else {
            shift = shift_value % alphabet.size();
            if (shift < 0) {
                shift += alphabet.size();
            }
        }
        
        // Сохраняем сгенерированный ключ в хранилище
        KeyStorage::getInstance()->setCaesarKey(get_key());
    }
    
    // Получение текущего ключа в виде строки
    std::string get_key() const {
        return "shift=" + std::to_string(shift);
    }
    
    // Установка ключа из строки
    bool set_key(const std::string& key) {
        size_t shiftPos = key.find("shift=");
        if (shiftPos != std::string::npos) {
            shift = std::stoi(key.substr(shiftPos + 6));
            shift %= alphabet.size();
            if (shift < 0) {
                shift += alphabet.size();
            }
            return true;
        }
        return false;
    }

    std::string encode(const std::string message) override {
        std::string encrypted;
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                // Шифруем символ путем сдвига индекса
                int newIdx = (idx + shift) % alphabet.size();
                encrypted += alphabet[newIdx];
            } else {
                // Символы, которых нет в алфавите, оставляем без изменений
                encrypted += c;
            }
        }
        
        // Сохраняем текущий ключ в хранилище
        KeyStorage::getInstance()->setCaesarKey(get_key());
        
        return encrypted;
    }

    std::string decode(const std::string message) override {
        // Проверяем наличие ключа в хранилище
        std::string stored_key = KeyStorage::getInstance()->getCaesarKey();
        if (stored_key.empty()) {
            throw std::runtime_error("Ошибка: Отсутствует ключ для шифра Цезаря. Расшифровка невозможна.");
        }
        
        // Устанавливаем ключ из хранилища
        if (!set_key(stored_key)) {
            throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Цезаря.");
        }
        
        std::string decrypted;
        int alphaSize = alphabet.size();
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                // Дешифруем символ путем обратного сдвига
                int newIdx = (idx - shift + alphaSize) % alphaSize;
                decrypted += alphabet[newIdx];
            } else {
                // Символы, которых нет в алфавите, оставляем без изменений
                decrypted += c;
            }
        }
        
        return decrypted;
    }
    
    // Метод шифрования с явным указанием ключа
    std::pair<std::string, std::string> encode_with_key(const std::string message) override {
        // Сохраняем текущий ключ перед шифрованием
        std::string key = get_key();
        
        // Шифруем сообщение
        std::string encrypted = encode(message);
        
        return {encrypted, key};
    }
    
    // Метод дешифрования с явным указанием ключа
    std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key) override {
        // Запоминаем текущий сдвиг
        int old_shift = shift;
        
        // Устанавливаем новый ключ
        if (!set_key(key)) {
            throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Цезаря.");
        }
        
        // Дешифруем сообщение
        std::string decrypted;
        int alphaSize = alphabet.size();
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                int newIdx = (idx - shift + alphaSize) % alphaSize;
                decrypted += alphabet[newIdx];
            } else {
                decrypted += c;
            }
        }
        
        // Сохраняем использованный ключ в хранилище
        KeyStorage::getInstance()->setCaesarKey(key);
        
        return {decrypted, key};
    }
};

#endif // CAESAR_CIPHER_H
