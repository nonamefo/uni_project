#include "../../project_libs.h"
#include "../basic_cripto.h"
#include "../key_storage.h"

#include "../Vigenere_Cipher.h"



// Функция для проверки, содержится ли символ в алфавите
bool vigenere_cipher::isInAlphabet(char c) const {
    return alphabet.find(c) != std::string::npos;
};
    
// Получение индекса символа в алфавите
int vigenere_cipher::getIndex(char c) const {
    auto it = std::find(alphabet.begin(), alphabet.end(), c);
    if (it != alphabet.end()) {
        return std::distance(alphabet.begin(), it);
    }
    return -1;
};
    
// Генерирует случайное ключевое слово
std::string vigenere_cipher::generateRandomKeyword(int length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, alphabet.size() - 1);
    
    std::string result;
    for (int i = 0; i < length; i++) {
        result += alphabet[dis(gen)];
    }
    return result;
};

vigenere_cipher::vigenere_cipher(const std::string& key) {
    // Алфавит включает английские и русские буквы, а также основные символы
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
                "0123456789 ,.;:!?-_\"'()[]{}";
    
    // Если ключ не предоставлен, генерируем случайное ключевое слово
    if (key.empty()) {
        // Генерируем ключевое слово длиной 5-10 символов
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(5, 10);
        int keyLength = dis(gen);
        
        keyword = generateRandomKeyword(keyLength);
    } else {
        // Используем предоставленный ключ
        keyword = key;
    }
    
    // Сохраняем сгенерированный ключ в хранилище
    KeyStorage::getInstance()->setVigenereKey(get_key());
};
    
// Получение текущего ключа в виде строки
std::string vigenere_cipher::get_key() const {
    return "keyword=" + keyword;
};

// Установка ключа из строки
bool vigenere_cipher::set_key(const std::string& key) {
    size_t keywordPos = key.find("keyword=");
    if (keywordPos != std::string::npos) {
        keyword = key.substr(keywordPos + 8);
        return true;
    }
    return false;
};

std::string vigenere_cipher::encode(const std::string message)  {
    if (keyword.empty()) {
        throw std::runtime_error("Ошибка: Пустое ключевое слово для шифра Виженера.");
    }
    
    std::string encrypted;
    size_t keywordIndex = 0;
    
    for (char c : message) {
        int charIndex = getIndex(c);
        if (charIndex != -1) {
            // Получаем смещение из ключевого слова
            char keyChar = keyword[keywordIndex % keyword.length()];
            int keyIndex = getIndex(keyChar);
            
            // Шифруем символ по формуле Виженера: (charIndex + keyIndex) % alphabetSize
            int newIndex = (charIndex + keyIndex) % alphabet.size();
            encrypted += alphabet[newIndex];
            
            // Переходим к следующему символу ключа
            keywordIndex++;
        } else {
            // Символы, которых нет в алфавите, оставляем без изменений
            encrypted += c;
        }
    }
    
    // Сохраняем ключ в хранилище
    KeyStorage::getInstance()->setVigenereKey(get_key());
    
    return encrypted;
};

std::string vigenere_cipher::decode(const std::string message)  {
    // Проверяем наличие ключа в хранилище
    std::string stored_key = KeyStorage::getInstance()->getVigenereKey();
    if (stored_key.empty()) {
        throw std::runtime_error("Ошибка: Отсутствует ключ для шифра Виженера. Расшифровка невозможна.");
    }
    
    // Устанавливаем ключ из хранилища
    if (!set_key(stored_key)) {
        throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Виженера.");
    }
    
    if (keyword.empty()) {
        throw std::runtime_error("Ошибка: Пустое ключевое слово для шифра Виженера.");
    }
    
    std::string decrypted;
    size_t keywordIndex = 0;
    
    for (char c : message) {
        int charIndex = getIndex(c);
        if (charIndex != -1) {
            // Получаем смещение из ключевого слова
            char keyChar = keyword[keywordIndex % keyword.length()];
            int keyIndex = getIndex(keyChar);
            
            // Дешифруем символ по формуле Виженера: (charIndex - keyIndex + alphabetSize) % alphabetSize
            int newIndex = (charIndex - keyIndex + alphabet.size()) % alphabet.size();
            decrypted += alphabet[newIndex];
            
            // Переходим к следующему символу ключа
            keywordIndex++;
        } else {
            // Символы, которых нет в алфавите, оставляем без изменений
            decrypted += c;
        }
    }
    
    return decrypted;
};

// Метод шифрования с явным указанием ключа
std::pair<std::string, std::string> vigenere_cipher::encode_with_key(const std::string message)  {
    // Сохраняем текущий ключ перед шифрованием
    std::string key = get_key();
    
    // Шифруем сообщение
    std::string encrypted = encode(message);
    
    return {encrypted, key};
};

// Метод дешифрования с явным указанием ключа
std::pair<std::string, std::string> vigenere_cipher::decode_with_key(const std::string message, const std::string key)  {
    // Запоминаем текущее ключевое слово
    std::string old_keyword = keyword;
    
    // Устанавливаем новый ключ
    if (!set_key(key)) {
        throw std::runtime_error("Ошибка: Неверный формат ключа для шифра Виженера.");
    }
    
    if (keyword.empty()) {
        throw std::runtime_error("Ошибка: Пустое ключевое слово для шифра Виженера.");
    }
    
    // Дешифруем сообщение
    std::string decrypted;
    size_t keywordIndex = 0;
    
    for (char c : message) {
        int charIndex = getIndex(c);
        if (charIndex != -1) {
            // Получаем смещение из ключевого слова
            char keyChar = keyword[keywordIndex % keyword.length()];
            int keyIndex = getIndex(keyChar);
            
            // Дешифруем символ по формуле Виженера
            int newIndex = (charIndex - keyIndex + alphabet.size()) % alphabet.size();
            decrypted += alphabet[newIndex];
            
            keywordIndex++;
        } else {
            decrypted += c;
        }
    }
    
    // Сохраняем использованный ключ в хранилище
    KeyStorage::getInstance()->setVigenereKey(key);
    
    return {decrypted, key};
};