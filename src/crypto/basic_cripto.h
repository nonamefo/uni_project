#ifndef BASIC_CRIPTO_H 
#define BASIC_CRIPTO_H

#include <string>

class basic_cripto {
public:
    virtual ~basic_cripto() = default; // Добавьте виртуальный деструктор

    // Основные методы
    virtual std::string decode(const std::string message) = 0;
    virtual std::string encode(const std::string message) = 0;

    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
               "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
               "0123456789 ,.;:!?-_\"'()[]{}";;

    // Методы для работы с ключами
    virtual std::string get_key() const = 0;  // Добавлен const
    virtual bool set_key(const std::string& key) = 0;

    // Методы с возвратом пары (результат + ключ)
    virtual std::pair<std::string, std::string> encode_with_key(const std::string message) = 0;
    virtual std::pair<std::string, std::string> decode_with_key(const std::string message, const std::string key) = 0;
};

#endif // BASIC_CRIPTO_H