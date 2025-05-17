#ifndef KEY_STORAGE_H
#define KEY_STORAGE_H

#include <string>

class KeyStorage {
private:
    static KeyStorage* instance;
    
    // Ключи для каждого шифровальщика
    std::string affine_key;
    std::string caesar_key;
    std::string substitution_key;
    std::string transposition_key;
    std::string vigenere_key;
    
    // Приватный конструктор (паттерн Singleton(это как я понял глобализация обьекта, что он ток один))
    KeyStorage() : 
        affine_key(""), 
        caesar_key(""),
        substitution_key(""),
        transposition_key(""),
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
    
    //Работа с ключом шифра замены
    void setSubstitutionKey(const std::string& key) { substitution_key = key; }
    std::string getSubstitutionKey() const { return substitution_key; }
    
    // Работа с ключом шифра перестановки
    void setTranspositionKey(const std::string& key) { transposition_key = key; }
    std::string getTranspositionKey() const { return transposition_key; }
    
    // Работа с ключом шифра Виженера
    void setVigenereKey(const std::string& key) { vigenere_key = key; }
    std::string getVigenereKey() const { return vigenere_key; }
    
    // Сброс всех ключей
    void resetAllKeys() {
        affine_key = "";
        caesar_key = "";
        substitution_key = "";
        transposition_key = "";
        vigenere_key = "";
    }
    
    //сохранение/загрузка ключей в файл (на будущее)
  //  bool saveToFile(const std::string& filename);
  //  bool loadFromFile(const std::string& filename);
};

// Инициализация статического экземпляра
KeyStorage* KeyStorage::instance = nullptr;

#endif // KEY_STORAGE_H
