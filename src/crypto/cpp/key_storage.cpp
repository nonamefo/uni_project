#include <mutex>

#include "../key_storage.h"


// Инициализация статического указателя и мьютекса
KeyStorage* KeyStorage::instance = nullptr;
std::mutex KeyStorage::mutex;

// Конструктор
KeyStorage::KeyStorage() :
    affine_key(""),
    caesar_key(""),
    playfair_key(""),
    polybius_key(""),
    vigenere_key("") {
}

// Деструктор
KeyStorage::~KeyStorage() {
    // Никаких операций удаления здесь быть не должно
}

// Потокобезопасное получение экземпляра с двойной проверкой
KeyStorage* KeyStorage::getInstance() {
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr) {
            instance = new KeyStorage();
        }
    }
    return instance;
}

// Метод освобождения ресурсов
void KeyStorage::destroyInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

// Методы для аффинного шифра
void KeyStorage::setAffineKey(const std::string& key) {
    affine_key = key;
}

std::string KeyStorage::getAffineKey() const {
    return affine_key;
}

// Методы для шифра Цезаря
void KeyStorage::setCaesarKey(const std::string& key) {
    caesar_key = key;
}

std::string KeyStorage::getCaesarKey() const {
    return caesar_key;
}

// Методы для шифра Плэйфера
void KeyStorage::setPlayfairKey(const std::string& key) {
    playfair_key = key;
}

std::string KeyStorage::getPlayfairKey() const {
    return playfair_key;
}

// Методы для шифра Полибиуса
void KeyStorage::setPolybiusKey(const std::string& key) {
    polybius_key = key;
}

std::string KeyStorage::getPolybiusKey() const {
    return polybius_key;
}

// Методы для шифра Виженера
void KeyStorage::setVigenereKey(const std::string& key) {
    vigenere_key = key;
}

std::string KeyStorage::getVigenereKey() const {
    return vigenere_key;
}

// Сброс всех ключей
void KeyStorage::resetAllKeys() {
    affine_key = "";
    caesar_key = "";
    playfair_key = "";
    polybius_key = "";
    vigenere_key = "";
}
