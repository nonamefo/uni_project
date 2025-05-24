#ifndef KEY_STORAGE_H
#define KEY_STORAGE_H

#include <string>
#include <mutex>

class KeyStorage {
private:
    static KeyStorage* instance;
    static std::mutex mutex;  // Мьютекс для потокобезопасности
    
    // Ключи для каждого шифровальщика
    std::string affine_key;
    std::string caesar_key;
    std::string playfair_key;
    std::string polybius_key;
    std::string vigenere_key;
    
    // Приватный конструктор (паттерн Singleton)
    KeyStorage();
        
public:
    // Получение экземпляра хранилища (потокобезопасный)
    static KeyStorage* getInstance();

    // Освобождение ресурсов
    static void destroyInstance();
    
    // Работа с ключом аффинного шифра
    void setAffineKey(const std::string& key);
    std::string getAffineKey() const;
    
    // Работа с ключом шифра Цезаря
    void setCaesarKey(const std::string& key);
    std::string getCaesarKey() const;
    
    // Работа с ключом шифра Плэйфера
    void setPlayfairKey(const std::string& key);
    std::string getPlayfairKey() const;
    
    // Работа с ключом шифра Полибиуса
    void setPolybiusKey(const std::string& key);
    std::string getPolybiusKey() const;
    
    // Работа с ключом шифра Виженера
    void setVigenereKey(const std::string& key);
    std::string getVigenereKey() const;
    
    // Сброс всех ключей
    void resetAllKeys();

    // Деструктор
    ~KeyStorage();

    // Запрещаем копирование
    KeyStorage(const KeyStorage&) = delete;
    KeyStorage& operator=(const KeyStorage&) = delete;
};

#endif // KEY_STORAGE_H
