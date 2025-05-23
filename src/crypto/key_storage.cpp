#ifndef KEY_STORAGE_H
#define KEY_STORAGE_H

#include <string>

class KeyStorage {
private:
    static KeyStorage* instance;#ifndef AFFINE_CIPHER_H
#define AFFINE_CIPHER_H

#include "../project_libs.hpp"
#include "basic_cripto.h"
#include "key_storage.h" // Используем ваше хранилище ключей

class affine_cipher : public basic_cripto {
private:
    int a; // Коэффициент
    int b; // Смещение
    int modSize; // Размер алфавита для модуля
    std::string alphabet; // Используемый алфавит

    // Функция для нахождения модульного обратного числа
    int modInverse(int a, int m) {
        a = a % m;
        for (int x = 1; x < m; x++) {
            if ((a * x) % m == 1) {
                return x;
            }
        }
        return 1; // Если обратного не существует
    }

    // Получение индекса символа в алфавите
    int getIndex(char c) {
        auto it = std::find(alphabet.begin(), alphabet.end(), c);
        if (it != alphabet.end()) {
            return std::distance(alphabet.begin(), it);
        }
        return -1;
    }

public:
    affine_cipher(int a_value = -1, int b_value = -1) {
        // Алфавит включает английские и русские буквы, а также основные символы
        alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                   "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                   "0123456789 ,.;:!?-_\"'()[]{}";
        
        modSize = alphabet.size();

        // Генерируем случайный ключ, если не предоставлен
        if (a_value == -1 || b_value == -1) {
            std::random_device rd;
            std::mt19937 gen(rd());
            
            // a должно быть взаимно простым с размером алфавита
            std::vector<int> validA;
            for (int i = 2; i < modSize; i++) {
                if (std::gcd(i, modSize) == 1) {
                    validA.push_back(i);
                }
            }
            
            std::uniform_int_distribution<> a_dis(0, validA.size() - 1);
            std::uniform_int_distribution<> b_dis(1, modSize - 1);
            
            a = validA[a_dis(gen)];
            b = b_dis(gen);
        } else {
            // Проверяем, что a взаимно просто с modSize
            if (std::gcd(a_value, modSize) != 1) {
                // Если нет, используем значение по умолчанию
                a = 5;
            } else {
                a = a_value;
            }
            b = b_value % modSize;
        }
        
        // Сохраняем сгенерированный ключ в хранилище
        KeyStorage::getInstance()->setAffineKey(get_key());
    }

    // Получение текущего ключа в виде строки
    std::string get_key() const {
        return "a=" + std::to_string(a) + ",b=" + std::to_string(b);
    }
    
    // Установка ключа из строки
    bool set_key(const std::string& key) {
        size_t aPos = key.find("a=");
        size_t bPos = key.find(",b=");
        
        if (aPos != std::string::npos && bPos != std::string::npos) {
            int new_a = std::stoi(key.substr(aPos + 2, bPos - aPos - 2));
            int new_b = std::stoi(key.substr(bPos + 3));
            
            // Проверяем, что a взаимно просто с modSize
            if (std::gcd(new_a, modSize) == 1) {
                a = new_a;
                b = new_b % modSize;
                return true;
            }
        }
        return false;
    }

    std::string encode(const std::string message) override {
        std::string encrypted;
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                // Шифруем символ по формуле аффинного шифра: E(x) = (ax + b) mod m
                int newIdx = (a * idx + b) % modSize;
                encrypted += alphabet[newIdx];
            } else {
                // Символы, которых нет в алфавите, оставляем без изменений
                encrypted += c;
            }
        }
        
        // Сохраняем текущий ключ в хранилище
        KeyStorage::getInstance()->setAffineKey(get_key());
        
        return encrypted;
    }

    std::string decode(const std::string message) override {
        // Проверяем наличие ключа в хранилище
        std::string stored_key = KeyStorage::getInstance()->getAffineKey();
        if (stored_key.empty()) {
            throw std::runtime_error("Ошибка: Отсутствует ключ для аффинного шифра. Расшифровка невозможна.");
        }
        
        // Устанавливаем ключ из хранилища
        if (!set_key(stored_key)) {
            throw std::runtime_error("Ошибка: Неверный формат ключа для аффинного шифра.");
        }
        
        std::string decrypted;
        int aInv = modInverse(a, modSize);
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                // Дешифруем символ по формуле: D(y) = a^(-1) * (y - b) mod m
                int newIdx = (aInv * ((idx - b) % modSize + modSize)) % modSize;
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
        // Запоминаем текущие значения ключа
        int old_a = a;
        int old_b = b;
        
        // Устанавливаем новый ключ
        if (!set_key(key)) {
            throw std::runtime_error("Ошибка: Неверный формат ключа для аффинного шифра.");
        }
        
        // Дешифруем сообщение
        std::string decrypted;
        int aInv = modInverse(a, modSize);
        
        for (char c : message) {
            int idx = getIndex(c);
            if (idx != -1) {
                int newIdx = (aInv * ((idx - b) % modSize + modSize)) % modSize;
                decrypted += alphabet[newIdx];
            } else {
                decrypted += c;
            }
        }
        
        // Сохраняем использованный ключ в хранилище
        KeyStorage::getInstance()->setAffineKey(key);
        
        // Возвращаем расшифрованное сообщение и использованный ключ
        return {decrypted, key};
    }
};

#endif // AFFINE_CIPHER_H

    
    // Ключи для каждого шифровальщика
    std::string affine_key;
    std::string caesar_key;
    std::string playfair_key;
    std::string polybius_key;
    std::string vigenere_key;
    
    // Приватный конструктор (паттерн Singleton(это как я понял глобализация обьекта, что он ток один))
    KeyStorage() : 
        affine_key(""), 
        caesar_key(""),
        playfair_key(""),
        polybius_key(""),
        vigenere_key("") {}
        
public:
    // Получение экземпляра хранилища
    static KeyStorage* getInstance() {
        if (!instance) {
            instance = new KeyStorage();
        }
        return instance;
    }
    
    // Работа с ключом аффинного шифра
    void setAffineKey(const std::string& key) { affine_key = key; }
    std::string getAffineKey() const { return affine_key; }
    
    // Работа с ключом шифра Цезаря
    void setCaesarKey(const std::string& key) { caesar_key = key; }
    std::string getCaesarKey() const { return caesar_key; }
    
    //Работа с ключом шифра Плэйфера
    void setPlayfairKey(const std::string& key) { playfair_key = key; }
    std::string getPlayfairKey() const { return playfair_key; }
    
    // Работа с ключом шифра Полибиуса
    void setPolybiusKey(const std::string& key) { polybius_key = key; }
    std::string getPolybiusKey() const { return polybius_key; }
    
    // Работа с ключом шифра Виженера
    void setVigenereKey(const std::string& key) { vigenere_key = key; }
    std::string getVigenereKey() const { return vigenere_key; }
    
    // Сброс всех ключей
    void resetAllKeys() {
        affine_key = "";
        caesar_key = "";
        playfair_key = "";
        polybius_key = "";
        vigenere_key = "";
    }
    
    //сохранение/загрузка ключей в файл (на будущее)
  //  bool saveToFile(const std::string& filename);
  //  bool loadFromFile(const std::string& filename);
};

// Инициализация статического экземпляра
KeyStorage* KeyStorage::instance = nullptr;

#endif // KEY_STORAGE_H
