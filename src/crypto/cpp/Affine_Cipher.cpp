#include "../../project_libs.h"
#include "../basic_cripto.h"
#include "../key_storage.h"

#include "../Affine_Cipher.h"

int affine_cipher::modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return 1; // Если обратного не существует
}

int affine_cipher::getIndex(char c) {
    auto it = std::find(alphabet.begin(), alphabet.end(), c);
    return (it != alphabet.end()) ? std::distance(alphabet.begin(), it) : -1;
}

// Конструктор без значений по умолчанию
affine_cipher::affine_cipher(int a_value, int b_value) {
    
    
    modSize = alphabet.size();

    if (a_value == -1 || b_value == -1) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::vector<int> validA;
        
        for (int i = 2; i < modSize; i++) {
            if (std::gcd(i, modSize) == 1) validA.push_back(i);
        }
        
        std::uniform_int_distribution<> a_dis(0, validA.size() - 1);
        std::uniform_int_distribution<> b_dis(1, modSize - 1);
        a = validA[a_dis(gen)];
        b = b_dis(gen);
    } else {
        a = (std::gcd(a_value, modSize) != 1) ? 5 : a_value;
        b = b_value % modSize;
    }
    
    KeyStorage::getInstance()->setAffineKey(get_key());
}

std::string affine_cipher::get_key() const {
    return "a=" + std::to_string(a) + ",b=" + std::to_string(b);
}

bool affine_cipher::set_key(const std::string& key) {
    size_t aPos = key.find("a=");
    size_t bPos = key.find(",b=");
    
    if (aPos != std::string::npos && bPos != std::string::npos) {
        int new_a = std::stoi(key.substr(aPos + 2, bPos - aPos - 2));
        int new_b = std::stoi(key.substr(bPos + 3));
        
        if (std::gcd(new_a, modSize) == 1) {
            a = new_a;
            b = new_b % modSize;
            return true;
        }
    }
    return false;
}

// Убраны ключевые слова override
std::string affine_cipher::encode(const std::string message) {
    std::string encrypted;
    for (char c : message) {
        int idx = getIndex(c);
        if (idx != -1) encrypted += alphabet[(a * idx + b) % modSize];
        else encrypted += c;
    }
    KeyStorage::getInstance()->setAffineKey(get_key());
    return encrypted;
}

std::string affine_cipher::decode(const std::string message) {
    std::string stored_key = KeyStorage::getInstance()->getAffineKey();
    if (stored_key.empty()) throw std::runtime_error("Ошибка: Отсутствует ключ для аффинного шифра.");
    
    if (!set_key(stored_key)) throw std::runtime_error("Ошибка: Неверный формат ключа.");
    
    std::string decrypted;
    int aInv = modInverse(a, modSize);
    
    for (char c : message) {
        int idx = getIndex(c);
        if (idx != -1) decrypted += alphabet[(aInv * (idx - b + modSize)) % modSize];
        else decrypted += c;
    }
    return decrypted;
}

std::pair<std::string, std::string> affine_cipher::encode_with_key(const std::string message) {
    return {encode(message), get_key()};
}

std::pair<std::string, std::string> affine_cipher::decode_with_key(const std::string message, const std::string key) {
    int old_a = a, old_b = b;
    if (!set_key(key)) throw std::runtime_error("Ошибка: Неверный формат ключа.");
    
    std::string decrypted = decode(message);
    KeyStorage::getInstance()->setAffineKey(key);
    a = old_a; b = old_b; // Восстановление исходного ключа
    
    return {decrypted, key};
}